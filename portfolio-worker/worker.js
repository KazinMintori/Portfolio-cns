/**
 * Portfolio Cloudflare Worker
 * ───────────────────────────
 * Lưu GitHub PAT trong Secrets (server-side), thay thế toàn bộ lời gọi
 * GitHub API từ trình duyệt. Client chỉ cần gửi EDIT_PASSWORD.
 *
 * Secrets (qua wrangler secret put):
 *   EDIT_PASSWORD  — mật khẩu chủ trang dùng để edit
 *   GH_PAT         — GitHub Personal Access Token (Contents R/W)
 *
 * Env vars (trong wrangler.toml):
 *   GH_OWNER, GH_REPO, GH_BRANCH, GH_UPLOAD_DIR, GH_DATA_PATH
 */

const CORS = {
  'Access-Control-Allow-Origin': '*',
  'Access-Control-Allow-Methods': 'GET, POST, PUT, DELETE, OPTIONS',
  'Access-Control-Allow-Headers': 'Content-Type, X-Edit-Password',
};

function json(data, status = 200) {
  return new Response(JSON.stringify(data), {
    status,
    headers: { 'Content-Type': 'application/json', ...CORS },
  });
}

function err(msg, status = 400) {
  return json({ error: msg }, status);
}

function checkAuth(request, env) {
  const pw = request.headers.get('X-Edit-Password') || '';
  return pw === env.EDIT_PASSWORD;
}

async function ghRequest(env, method, path, body) {
  // NOTE: ?ref= chỉ hợp lệ cho GET. PUT dùng branch trong body → bỏ ?ref khỏi URL chung,
  // GET vẫn cần nên ta append có điều kiện.
  const ref = method === 'GET' ? `?ref=${env.GH_BRANCH}` : '';
  const url = `https://api.github.com/repos/${env.GH_OWNER}/${env.GH_REPO}/contents/${path}${ref}`;
  const opts = {
    method,
    headers: {
      Authorization: `token ${env.GH_PAT}`,
      Accept: 'application/vnd.github+json',
      'Content-Type': 'application/json',
      'User-Agent': 'portfolio-worker/1.0',
    },
  };
  if (body) opts.body = JSON.stringify(body);
  return fetch(url, opts);
}

async function ghGetSha(env, path) {
  try {
    const r = await ghRequest(env, 'GET', path);
    if (!r.ok) return null;
    const d = await r.json();
    return d.sha || null;
  } catch { return null; }
}

async function ghPutFile(env, path, contentBase64, message) {
  const sha = await ghGetSha(env, path);
  const body = { message: message || ('update ' + path), content: contentBase64, branch: env.GH_BRANCH };
  if (sha) body.sha = sha;
  const res = await ghRequest(env, 'PUT', path, body);
  if (!res.ok) {
    const t = await res.text().catch(() => '');
    throw new Error(`GitHub ${res.status}: ${t.slice(0, 300)}`);
  }
  return await res.json();
}

async function handleGetData(env) {
  try {
    const rawUrl = `https://raw.githubusercontent.com/${env.GH_OWNER}/${env.GH_REPO}/${env.GH_BRANCH}/${env.GH_DATA_PATH}`;
    const r = await fetch(rawUrl + '?t=' + Date.now(), { headers: { 'Cache-Control': 'no-cache' } });
    if (r.ok) {
      const text = await r.text();
      if (text.trim()) return new Response(text, { headers: { 'Content-Type': 'application/json', ...CORS } });
    }
  } catch { }
  try {
    const r = await fetch(
      `https://api.github.com/repos/${env.GH_OWNER}/${env.GH_REPO}/contents/${env.GH_DATA_PATH}?ref=${env.GH_BRANCH}`,
      { headers: { Authorization: `token ${env.GH_PAT}`, Accept: 'application/vnd.github.raw', 'User-Agent': 'portfolio-worker/1.0' } }
    );
    if (r.ok) return new Response(await r.text(), { headers: { 'Content-Type': 'application/json', ...CORS } });
  } catch { }
  return err('Không đọc được data.json', 502);
}

async function handleWriteData(request, env) {
  if (!checkAuth(request, env)) return err('Sai mật khẩu', 401);
  const body = await request.json().catch(() => null);
  if (!body || !body.data) return err('Thiếu trường data');
  const json_str = JSON.stringify(body.data, null, 2);
  const bytes = new TextEncoder().encode(json_str);
  const b64 = btoa(String.fromCharCode(...bytes));
  try {
    await ghPutFile(env, env.GH_DATA_PATH, b64, 'update data.json');
    return json({ ok: true });
  } catch (e) {
    return err(e.message, 502);
  }
}

async function handleUpload(request, env) {
  if (!checkAuth(request, env)) return err('Sai mật khẩu', 401);

  let contentBase64, filename, ext, prefix;

  const ct = request.headers.get('Content-Type') || '';
  if (ct.includes('multipart/form-data')) {
    /* ── Chế độ binary multipart: tránh giới hạn base64 trên free plan ── */
    let formData;
    try { formData = await request.formData(); } catch (e) { return err('formData lỗi: ' + e.message); }
    const fileField = formData.get('file');
    if (!fileField || typeof fileField.arrayBuffer !== 'function') return err('Thiếu trường file');
    filename = formData.get('filename') || fileField.name || 'upload.bin';
    prefix = formData.get('prefix') || 'upload';
    ext = (formData.get('ext') || filename.split('.').pop() || 'bin').toLowerCase();
    const buf = await fileField.arrayBuffer();
    /* ArrayBuffer → base64 an toàn với file lớn */
    const bytes = new Uint8Array(buf);
    let bin = '';
    const CHUNK = 8192;
    for (let i = 0; i < bytes.length; i += CHUNK) {
      bin += String.fromCharCode(...bytes.subarray(i, i + CHUNK));
    }
    contentBase64 = btoa(bin);
  } else {
    /* ── Chế độ JSON base64 cũ (ảnh nhỏ, avatar, v.v.) ── */
    const body = await request.json().catch(() => null);
    if (!body || !body.contentBase64 || !body.filename) return err('Thiếu trường bắt buộc');
    contentBase64 = body.contentBase64;
    filename = body.filename;
    ext = (body.ext || filename.split('.').pop() || 'bin').toLowerCase();
    prefix = body.prefix || 'upload';
  }

  const stem = (filename.replace(/\.[^.]+$/, '') || 'file')
    .replace(/[^a-zA-Z0-9_-]/g, '_').slice(0, 40) || 'file';
  const safePrefix = prefix.replace(/[^a-zA-Z0-9_-]/g, '_');
  const path = `${env.GH_UPLOAD_DIR}/${safePrefix}-${stem}-${Date.now()}.${ext}`;
  try {
    await ghPutFile(env, path, contentBase64, 'upload: ' + path);
    const NON_CDN = ['docx', 'doc', 'pdf', 'zip', 'xlsx', 'pptx'];
    const url = NON_CDN.includes(ext)
      ? `https://raw.githubusercontent.com/${env.GH_OWNER}/${env.GH_REPO}/${env.GH_BRANCH}/${path}`
      : `https://cdn.jsdelivr.net/gh/${env.GH_OWNER}/${env.GH_REPO}@${env.GH_BRANCH}/${path}`;
    return json({ ok: true, url, path });
  } catch (e) {
    return err(e.message, 502);
  }
}

async function handleAuthCheck(request, env) {
  return json({ ok: checkAuth(request, env) });
}

export default {
  async fetch(request, env) {
    if (request.method === 'OPTIONS') {
      return new Response(null, { status: 204, headers: CORS });
    }
    const url = new URL(request.url);
    const path = url.pathname.replace(/\/+$/, '');
    if (path === '/api/data' && request.method === 'GET') return handleGetData(env);
    if (path === '/api/data' && request.method === 'POST') return handleWriteData(request, env);
    if (path === '/api/upload' && request.method === 'POST') return handleUpload(request, env);
    if (path === '/api/auth/check' && request.method === 'POST') return handleAuthCheck(request, env);
    return new Response('Portfolio Worker — OK', { headers: { ...CORS, 'Content-Type': 'text/plain' } });
  },
};