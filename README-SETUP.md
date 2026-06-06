# Hướng dẫn cài đặt – Portfolio lưu trữ trên GitHub

Mọi ảnh, ảnh nền slide, và file Word khi bạn upload sẽ được **commit thẳng vào repo `kazinmintori/Portfolio-cns`** trong thư mục `assets/uploads/`, và load qua jsDelivr CDN. Trạng thái (URL nào ứng với slot nào) lưu trong `data.json` ở gốc repo.

## 1. Đặt file vào repo

Cấu trúc thư mục cần có ở repo `kazinmintori/Portfolio-cns`:

```
Portfolio-cns/
├── index.html              ← thay bằng file mới
├── data.json               ← thêm mới (template rỗng)
└── assets/
    └── uploads/
        └── .gitkeep        ← giữ folder tồn tại
```

Bạn có thể commit `data.json` rỗng và `assets/uploads/.gitkeep` ngay bây giờ – mọi upload về sau sẽ tự tạo file trong folder này.

## 2. Tạo Personal Access Token (PAT)

1. Vào https://github.com/settings/personal-access-tokens/new (Fine-grained token)
2. Đặt tên (vd: `portfolio-edit`), expiration tùy ý (90 ngày, 1 năm, hoặc no-expiration nếu bạn được phép)
3. **Repository access**: chọn `Only select repositories` → chọn `Portfolio-cns`
4. **Permissions → Repository permissions → Contents**: `Read and write`
5. Generate → copy token (bắt đầu bằng `github_pat_...`)

## 3. Cấu hình token trong trang

1. Mở https://kazinmintori.github.io/Portfolio-cns/
2. Bạn sẽ thấy nút `⚠️ Cấu hình GitHub` ở góc phải dưới (chỉ hiện khi `editMode = true`)
3. Bấm vào, dán token, Lưu.
4. Token lưu trong `localStorage` trên máy bạn — **không bao giờ** được commit lên repo.

Sau khi cấu hình, nút sẽ thành `☁️ GitHub`. Mọi upload sau đó đều được commit thẳng lên repo.

## 4. Cách hoạt động

- **Khi visitor mở trang**: trang fetch `data.json` (same-origin, GitHub Pages CDN → cực nhanh), rồi tải các asset qua jsDelivr CDN. Không cần token, không cần đăng nhập.
- **Khi bạn upload** (cần token): file được PUT lên `assets/uploads/<prefix>-<tên>-<timestamp>.<ext>` qua GitHub Contents API, sau đó `data.json` được cập nhật. Tất cả là commit thật trong git history.
- **jsDelivr CDN URL** dạng `https://cdn.jsdelivr.net/gh/kazinmintori/Portfolio-cns@main/assets/uploads/...` — phục vụ ngay sau khi commit (không cần đợi Pages rebuild), cache global rất nhanh.
- **Debounce**: nếu bạn thao tác nhiều liên tiếp (vd: upload 5 ảnh nhanh), `data.json` chỉ được commit 1 lần sau 600ms im lặng → tránh spam commit.

## 5. Lưu ý

- **Mỗi upload = 1 commit + file vào repo**. Sau nhiều upload, repo có thể to. Khi cần dọn, xóa file trong `assets/uploads/` và cập nhật `data.json` thủ công.
- **Giới hạn**: file < 25 MB qua Contents API (đủ thoải mái cho ảnh và docx). Repo nên < 1 GB.
- **Rate limit**: 5000 request/giờ với PAT đã auth — không lo.
- **Bảo mật**: token chỉ ở máy của bạn. Nếu lộ, vào https://github.com/settings/tokens revoke ngay.
- **Đổi branch**: nếu Pages của bạn build từ `gh-pages` thay vì `main`, sửa `GH_BRANCH` ở đầu phần script trong `index.html`.

## 6. Nếu mở bằng trình duyệt khác bị mất ảnh

Nếu một trình duyệt thấy ảnh nhưng trình duyệt khác không thấy, gần như chắc chắn ảnh đang chỉ nằm trong bộ nhớ local của trình duyệt cũ (`localStorage`/IndexedDB), chưa được ghi vào repo.

Kiểm tra nhanh:

1. Vào repo GitHub và mở `data.json`.
2. Nếu `"slots": {}` vẫn rỗng, ảnh chưa được public cho người xem khác.
3. Sau khi upload ảnh từ trang, phải thấy thêm commit mới cho file ảnh trong `assets/uploads/` và commit cập nhật `data.json`.
4. Nếu trang báo lỗi `data.json chưa được cập nhật`, tạo lại token với quyền **Contents: Read and write**, đúng repo `Portfolio-cns`, rồi upload lại ảnh.

## 7. Đổi chủ sở hữu / repo

Nếu fork sang tài khoản khác, sửa 3 dòng đầu trong script:
```js
const GH_OWNER  = 'your-username';
const GH_REPO   = 'your-repo';
const GH_BRANCH = 'main';
```
