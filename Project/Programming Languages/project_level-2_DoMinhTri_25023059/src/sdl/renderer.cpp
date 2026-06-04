#include "renderer.h"

#include <SDL2/SDL_image.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"
#include "../utils/logger.h"

static const SDL_Color BG = {15, 15, 25, 255};
static const SDL_Color PANEL = {35, 35, 50, 255};
static const SDL_Color BORDER = {160, 160, 190, 255};
static const SDL_Color WHITE = {255, 255, 255, 255};
static const SDL_Color GRAY = {140, 140, 155, 255};
static const SDL_Color CYAN = {0, 210, 210, 255};
static const SDL_Color RED = {220, 75, 75, 255};
static const SDL_Color BLUE = {75, 145, 220, 255};
static const SDL_Color YELLOW = {255, 215, 0, 255};
static const SDL_Color GREEN = {80, 200, 120, 255};
static const SDL_Color DIM = {80, 80, 100, 255};

void SDLRenderer::loadBg(const std::string &path)
{
    SDL_Surface *surf = IMG_Load(path.c_str());
    if (!surf)
    {
        Logger::log("Background load failed", Logger::Level::WARNING);
        return;
    }
    if (bgTexture)
        SDL_DestroyTexture(bgTexture);
    bgTexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void SDLRenderer::drawBg(Uint8 alpha)
{
    if (!bgTexture || !useBgImage)
        return;
    SDL_SetTextureAlphaMod(bgTexture, alpha);
    SDL_Rect dst = {0, 0, sw, sh};
    SDL_RenderCopy(renderer, bgTexture, nullptr, &dst);
}

void SDLRenderer::initStars()
{
    std::srand(42);
    stars.clear();
    for (int i = 0; i < 120; i++)
    {
        Star s;
        s.x = (float)(std::rand() % sw);
        s.y = (float)(std::rand() % sh);
        s.speed = 0.1f + (std::rand() % 30) / 100.f;
        s.brightness = 0.3f + (std::rand() % 70) / 100.f;
        s.size = 1 + std::rand() % 2;
        stars.push_back(s);
    }
}

void SDLRenderer::drawStarField()
{
    SDL_SetRenderDrawColor(renderer, BG.r, BG.g, BG.b, 255);
    SDL_RenderClear(renderer);
    for (auto &s : stars)
    {
        s.y += s.speed;
        if (s.y > sh)
        {
            s.y = 0;
            s.x = (float)(std::rand() % sw);
        }
        float twinkle = s.brightness + 0.2f * sin(s.y * 0.05f);
        Uint8 bright = (Uint8)(std::min(1.f, twinkle) * 255);
        SDL_SetRenderDrawColor(renderer, bright, bright, bright + 30, 255);
        SDL_Rect r = {(int)s.x, (int)s.y, s.size, s.size};
        SDL_RenderFillRect(renderer, &r);
    }
}

void SDLRenderer::drawGradientBg(SDL_Color top, SDL_Color bot)
{
    for (int y = 0; y < sh; y++)
    {
        float t = (float)y / sh;
        Uint8 r = (Uint8)(top.r + (bot.r - top.r) * t);
        Uint8 g = (Uint8)(top.g + (bot.g - top.g) * t);
        Uint8 b = (Uint8)(top.b + (bot.b - top.b) * t);
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, sw, y);
    }
}

void SDLRenderer::drawGridBg()
{
    SDL_SetRenderDrawColor(renderer, BG.r, BG.g, BG.b, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 35, 35, 55, 255);
    int step = 40;
    for (int x = 0; x < sw; x += step)
        SDL_RenderDrawLine(renderer, x, 0, x, sh);
    for (int y = 0; y < sh; y += step)
        SDL_RenderDrawLine(renderer, 0, y, sw, y);
}

void SDLRenderer::spawnParticles(int cx, int cy, SDL_Color col, int count)
{
    for (int i = 0; i < count; i++)
    {
        Particle p;
        p.x = (float)cx;
        p.y = (float)cy;
        float angle = (float)(std::rand() % 360) * (float)M_PI / 180.f;
        float speed = 1.f + (std::rand() % 5);
        p.vx = cos(angle) * speed;
        p.vy = sin(angle) * speed;
        p.color = col;
        p.maxLife = p.life = 30 + std::rand() % 30;
        particles.push_back(p);
    }
}

void SDLRenderer::drawParticles()
{
    for (auto &p : particles)
    {
        if (p.life <= 0)
            continue;
        float t = (float)p.life / p.maxLife;
        Uint8 a = (Uint8)(t * 255);
        SDL_SetRenderDrawColor(renderer, p.color.r, p.color.g, p.color.b, a);
        SDL_Rect r = {(int)p.x, (int)p.y, 4, 4};
        SDL_RenderFillRect(renderer, &r);
        p.x += p.vx;
        p.y += p.vy;
        p.vy += 0.1f;
        p.life--;
    }
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](const Particle &p)
                       { return p.life <= 0; }),
        particles.end());
}

SDLRenderer::SDLRenderer() : I_Renderer() {}
SDLRenderer::~SDLRenderer() { close(); }

void SDLRenderer::rframe() { SDL_RenderPresent(renderer); }

void SDLRenderer::drawRect(int x, int y, int w, int h, SDL_Color c, bool fill)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_Rect r = {x, y, w, h};
    if (fill)
        SDL_RenderFillRect(renderer, &r);
    else
        SDL_RenderDrawRect(renderer, &r);
}

void SDLRenderer::drawTxt(const std::string &s, int x, int y, SDL_Color c, TTF_Font *f)
{
    if (!f || s.empty())
        return;
    SDL_Surface *sur = TTF_RenderText_Blended(f, s.c_str(), c);
    if (!sur)
        return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sur);
    SDL_Rect dst = {x, y, sur->w, sur->h};
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
    SDL_FreeSurface(sur);
    SDL_DestroyTexture(tex);
}

void SDLRenderer::drawTxtC(const std::string &s, int cx, int y, SDL_Color c, TTF_Font *f)
{
    if (!f || s.empty())
        return;
    int tw, th;
    TTF_SizeText(f, s.c_str(), &tw, &th);
    drawTxt(s, cx - tw / 2, y, c, f);
}

void SDLRenderer::drawInputBox(int y)
{
    int cx = sw / 2, bw = 300, bh = 50;
    drawRect(cx - bw / 2, y, bw, bh, {50, 50, 70, 255}, true);
    drawRect(cx - bw / 2, y, bw, bh, BORDER, false);
    std::string disp = tmpInput.empty() ? "_" : tmpInput + "_";
    drawTxtC(disp, cx, y + 10, WHITE, fMedium);
    drawTxtC(std::format("Range: {} - {}", inMin, inMax), cx, y + bh + 8, GRAY, fSmall);
}

void SDLRenderer::drawButton(const std::string &label, int cx, int y,
                             int w, int h, bool selected, SDL_Color col)
{
    SDL_Color bg = selected ? SDL_Color{80, 80, 120, 255} : col;
    SDL_Color bord = selected ? CYAN : BORDER;
    drawRect(cx - w / 2, y, w, h, bg, true);
    drawRect(cx - w / 2, y, w, h, bord, false);
    if (selected)
    {
        drawRect(cx - w / 2 - 2, y - 2, w + 4, h + 4, {0, 210, 210, 60}, false);
    }
    drawTxtC(label, cx, y + h / 2 - 9, selected ? CYAN : WHITE, fMedium);
}

void SDLRenderer::animatePlacePiece(int row, int col, char symbol)
{
    int pad = 60, bp = std::min(sw, sh) - pad * 2;
    int cs = bp / boardSize;
    int sx = (sw - bp) / 2, sy = (sh - bp) / 2;
    int cx = sx + col * cs + cs / 2;
    int cy = sy + row * cs + cs / 2;
    int r = cs / 2 - cs / 5;

    SDL_Color col1 = (symbol == 'X') ? RED : BLUE;

    this->board[row][col] = symbol;

    for (int frame = 0; frame < 8; frame++)
    {
        drawGridBg();
        drawBg(80);
        drawRect(sx, sy, bp, bp, {0, 0, 0, 160}, true);
        renderBoard(board);
        float t = (float)frame / 7.f;
        float glow = 1.f - t * 0.6f;
        int rr = (int)(r * (0.4f + 0.6f * t));

        SDL_Color gc = {
            (Uint8)std::min(255, (int)(col1.r + (255 - col1.r) * glow * 0.6f)),
            (Uint8)std::min(255, (int)(col1.g + (255 - col1.g) * glow * 0.2f)),
            (Uint8)std::min(255, (int)(col1.b + (255 - col1.b) * glow * 0.6f)),
            220};

        SDL_SetRenderDrawColor(renderer, gc.r, gc.g, gc.b, gc.a);
        if (symbol == 'X')
        {
            for (int tt = -3; tt <= 3; tt++)
            {
                SDL_RenderDrawLine(renderer, cx - rr + tt, cy - rr, cx + rr + tt, cy + rr);
                SDL_RenderDrawLine(renderer, cx + rr + tt, cy - rr, cx - rr + tt, cy + rr);
            }
        }
        else
        {
            for (int deg = 0; deg < 360; deg++)
            {
                float a1 = deg * (float)M_PI / 180.f;
                float a2 = (deg + 1) * (float)M_PI / 180.f;
                SDL_RenderDrawLine(renderer,
                                   cx + (int)(rr * cos(a1)), cy + (int)(rr * sin(a1)),
                                   cx + (int)(rr * cos(a2)), cy + (int)(rr * sin(a2)));
            }
        }
        rframe();
        SDL_Delay(25);
    }
}

void SDLRenderer::animateWinLine(const WinLine *wl)
{
    if (!wl || wl->cells.empty())
        return;
    int pad = 60, bp = std::min(sw, sh) - pad * 2;
    int cs = bp / boardSize;
    int sx = (sw - bp) / 2, sy = (sh - bp) / 2;

    for (int pulse = 0; pulse < 6; pulse++)
    {
        float t = (float)(pulse % 3) / 2.f;
        Uint8 a = (Uint8)(100 + 155 * t);
        for (auto &[r, c] : wl->cells)
            drawRect(sx + c * cs + 2, sy + r * cs + 2, cs - 4, cs - 4, {255, 215, 0, a}, true);
        rframe();
        SDL_Delay(120);
    }
}

void SDLRenderer::init(const RunConfig &config)
{
    sw = config.screenWidth;
    sh = config.screenHeight;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
    window = SDL_CreateWindow(
        std::format("Tic-tac-toe [v{}]", VERSION).c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        sw, sh, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    fLarge = TTF_OpenFont("assets/PressStart2P-Regular.ttf", 22);
    fMedium = TTF_OpenFont("assets/PressStart2P-Regular.ttf", 14);
    fSmall = TTF_OpenFont("assets/PressStart2P-Regular.ttf", 10);
    fXLarge = TTF_OpenFont("assets/PressStart2P-Regular.ttf", 40);
    if (!fLarge)
        Logger::log("Font load failed", Logger::Level::WARNING);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    initStars();
    bgPaths = {"assets/bg1.jpg", "assets/bg2.jpg", "assets/bg3.jpg", "assets/bg4.jpg", "assets/bg5.jpg"};
    loadBg(bgPaths[0]);
    SDL_Surface *logoSurf = IMG_Load("assets/logo.png");
    if (logoSurf)
    {
        logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurf);
        SDL_FreeSurface(logoSurf);
    }
    else
    {
        Logger::log("Logo load failed", Logger::Level::WARNING);
    }
    Logger::log("SDLRenderer initialized", Logger::Level::INFO);
}

void SDLRenderer::close()
{
    if (fLarge)
    {
        TTF_CloseFont(fLarge);
        fLarge = nullptr;
    }
    if (fMedium)
    {
        TTF_CloseFont(fMedium);
        fMedium = nullptr;
    }
    if (fSmall)
    {
        TTF_CloseFont(fSmall);
        fSmall = nullptr;
    }
    TTF_Quit();
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (bgTexture)
    {
        SDL_DestroyTexture(bgTexture);
        bgTexture = nullptr;
    }
    if (logoTexture)
    {
        SDL_DestroyTexture(logoTexture);
        logoTexture = nullptr;
    }
    SDL_Quit();
    Logger::log("SDLRenderer closed", Logger::Level::INFO);
}

void SDLRenderer::clearScreen()
{
    SDL_SetRenderDrawColor(renderer, BG.r, BG.g, BG.b, 255);
    SDL_RenderClear(renderer);
}

void SDLRenderer::nextBg()
{
    curBgIdx = (curBgIdx + 1) % bgPaths.size();
    loadBg(bgPaths[curBgIdx]);
}
void SDLRenderer::prevBg()
{
    curBgIdx = (curBgIdx - 1 + bgPaths.size()) % bgPaths.size();
    loadBg(bgPaths[curBgIdx]);
}

void SDLRenderer::drawLogoXO()
{
    if (!logoTexture)
        return;

    int cx = sw / 2;
    int cy = 160;
    int lw = 520;
    int lh = 290;

    SDL_Rect dst = {cx - lw / 2, cy - lh / 2, lw, lh};
    SDL_RenderCopy(renderer, logoTexture, nullptr, &dst);
}
void SDLRenderer::showMainMenu(int selected)
{
    drawStarField();
    drawBg(160);
    drawLogoXO();

    std::string ostName = audio ? audio->getCurOstName() : "";
    if (!ostName.empty())
        drawTxtC(ostName, sw / 2, sh - 30, GRAY, fSmall);

    int bw = 300, bh = 46, cx = sw / 2;
    int startY = 280;

    drawButton("PLAY", cx, startY, bw, bh, selected == 0, DIM);
    drawButton("SETTINGS", cx, startY + 65, bw, bh, selected == 1, DIM);
    drawButton("ABOUT", cx, startY + 130, bw, bh, selected == 2, DIM);
    drawButton("QUIT", cx, startY + 195, bw, bh, selected == 3, {80, 30, 30, 255});

    rframe();
}
void SDLRenderer::showSettings()
{
    drawGradientBg({20, 10, 40, 255}, {10, 30, 60, 255});

    drawTxtC("SETTINGS", sw / 2, 40, CYAN, fLarge);

    if (audio)
    {
        drawTxtC("OST", sw / 2, 110, WHITE, fMedium);
        std::string ostLine = std::format("<  {}  >", audio->getCurOstName());
        drawTxtC(ostLine, sw / 2, 140, YELLOW, fMedium);
        drawTxtC("Left/Right arrow to change", sw / 2, 170, GRAY, fSmall);

        drawTxtC("VOLUME", sw / 2, 220, WHITE, fMedium);
        int vol = audio->getVolume();
        int barW = 300, barH = 16;
        int bx = sw / 2 - barW / 2, by = 250;
        drawRect(bx, by, barW, barH, {40, 40, 60, 255}, true);
        drawRect(bx, by, (int)(barW * vol / 100.0), barH, CYAN, true);
        drawRect(bx, by, barW, barH, BORDER, false);
        drawTxtC(std::format("{}%", vol), sw / 2, by + barH + 6, WHITE, fSmall);
        drawTxtC("Up/Down arrow to adjust", sw / 2, by + barH + 22, GRAY, fSmall);
    }

    if (bgTexture)
    {
        int pw = 200, ph = 112;
        int px = sw / 2 - pw / 2, py = sh - 240;
        std::string bgLine = std::format("<  bg {}  >", curBgIdx + 1);
        drawTxtC(bgLine, sw / 2, py - 40, WHITE, fSmall);
        SDL_SetTextureAlphaMod(bgTexture, 220);
        SDL_Rect dst = {px, py, pw, ph};
        SDL_RenderCopy(renderer, bgTexture, nullptr, &dst);
        drawRect(px, py, pw, ph, BORDER, false);
        drawTxtC("BACKGROUND", sw / 2, py - 22, WHITE, fSmall);
        std::string bgStatus = useBgImage ? "ON  (B to toggle)" : "OFF (B to toggle)";
        drawTxtC(bgStatus, sw / 2, py + ph + 6, useBgImage ? GREEN : GRAY, fSmall);
    }

    drawButton("BACK", sw / 2, sh - 75, 200, 42, false, DIM);
    drawTxtC("Press ESC or BACK to return", sw / 2, sh - 25, GRAY, fSmall);

    rframe();
}

void SDLRenderer::showAbout()
{
    drawGradientBg({10, 20, 50, 255}, {30, 10, 50, 255});

    drawTxtC("ABOUT", sw / 2, 60, CYAN, fLarge);

    int cx = sw / 2, y = 180;
    drawTxtC("Tic-tac-toe", cx, y, WHITE, fMedium);
    drawTxtC("OOP Project - Level 2", cx, y + 40, GRAY, fSmall);
    drawTxtC("By: Do Minh Tri", cx, y + 80, WHITE, fSmall);
    drawTxtC("MSSV: 25023059", cx, y + 110, GRAY, fSmall);
    drawTxtC("VNU - UET", cx, y + 150, GRAY, fSmall);

    drawTxtC("OST by Toby Fox", cx, y + 210, {150, 100, 200, 255}, fSmall);
    drawTxtC("SFX from freesound.org", cx, y + 235, {150, 100, 200, 255}, fSmall);

    drawButton("BACK", sw / 2, sh - 100, 200, 48, false, DIM);
    drawTxtC("Press ESC to return", sw / 2, sh - 35, GRAY, fSmall);

    rframe();
}

void SDLRenderer::setTempInput(const std::string &s)
{
    tmpInput = s;
    refreshMenu();
}
void SDLRenderer::setInputRange(int mn, int mx)
{
    inMin = mn;
    inMax = mx;
}
void SDLRenderer::refreshMenu() { showSelectMenu(curMenuType, curCtx); }

void SDLRenderer::showSelectMenu(SelectType t, int ctx)
{
    curMenuType = t;
    curCtx = ctx;

    if (t == SelectType::PLAYER_UI)
    {
        drawGridBg();
        renderBoard(this->board);
        SDL_Color col = (curPlayer == 0) ? RED : BLUE;
        drawRect(15, 15, 30, 30, col, true);
        std::string lbl = std::format("Player {} ({})", curPlayer + 1, curPlayer == 0 ? "X" : "O");
        drawTxt(lbl, 55, 20, WHITE, fMedium);
        drawTxtC("Click a cell to place your piece", sw / 2, sh - 30, GRAY, fSmall);
        rframe();
        return;
    }

    clearScreen();

    int px = sw / 4, py = sh / 4, pw = sw / 2, ph = sh / 2;
    drawRect(px, py, pw, ph, PANEL, true);
    drawRect(px, py, pw, ph, BORDER, false);

    int cx = sw / 2, top = py + 20;

    switch (t)
    {
    case SelectType::TITLE_UI:
        drawTxtC("TIC TAC TOE", cx, top, CYAN, fLarge);
        drawTxtC("Press any key to start", cx, top + 60, GRAY, fSmall);
        break;

    case SelectType::SIZE_UI:
        drawTxtC("BOARD SIZE", cx, top, WHITE, fLarge);
        drawTxtC(std::format("Enter value ({}-{})", BOARD_N_MIN, BOARD_N_MAX), cx, top + 50, GRAY, fMedium);
        drawInputBox(top + 100);
        drawTxtC("Number keys then ENTER", cx, top + 200, GRAY, fSmall);
        break;

    case SelectType::GOAL_UI:
        drawTxtC("WIN CONDITION", cx, top, WHITE, fLarge);
        drawTxtC(std::format("Enter goal (3-{})", std::min(ctx, 5)), cx, top + 50, GRAY, fMedium);
        drawInputBox(top + 100);
        drawTxtC("Number keys then ENTER", cx, top + 200, GRAY, fSmall);
        break;
    case SelectType::GAME_MODE_UI:
        drawTxtC("GAME MODE", cx, top, WHITE, fLarge);
        drawButton("1  PvP  Player vs Player", cx, top + 60, 380, 48, false, {60, 100, 60, 255});
        drawButton("2  PvE  Player vs Bot", cx, top + 120, 380, 48, false, {60, 60, 100, 255});
        drawButton("3  EvE  Bot vs Bot", cx, top + 180, 380, 48, false, {100, 60, 60, 255});
        drawInputBox(top + 250);
        break;

    case SelectType::BOT_LEVEL_UI:
    case SelectType::MUL_BOT_LEVEL_UI:
    {
        std::string title = (t == SelectType::BOT_LEVEL_UI)
                                ? "BOT LEVEL"
                                : std::format("BOT {} LEVEL", ctx + 1);
        drawTxtC(title, cx, top, WHITE, fLarge);
        drawTxtC("1: EASY", cx, top + 55, GRAY, fMedium);
        drawTxtC("2: MEDIUM", cx, top + 85, GRAY, fMedium);
        drawTxtC("3: HARD", cx, top + 115, GRAY, fMedium);
        drawInputBox(top + 160);
        break;
    }

    default:
        break;
    }

    rframe();
}

void SDLRenderer::showInvalidSelect(SelectType t, int ctx)
{
    for (int i = 0; i < 2; i++)
    {
        drawRect(sw / 4 - 4, sh / 4 - 4, sw / 2 + 8, sh / 2 + 8, {200, 0, 0, 100}, true);
        rframe();
        SDL_Delay(100);
        refreshMenu();
        SDL_Delay(100);
    }
    tmpInput.clear();
    refreshMenu();
}

void SDLRenderer::showValidSelect(SelectType t, int ctx)
{
    drawRect(sw / 4 - 4, sh / 4 - 4, sw / 2 + 8, sh / 2 + 8, {0, 180, 0, 80}, true);
    rframe();
    SDL_Delay(80);
}

void SDLRenderer::renderBoard(const char src[][BOARD_N_MAX])
{
    int pad = 60, bp = std::min(sw, sh) - pad * 2;
    int cs = bp / boardSize;
    int sx = (sw - bp) / 2, sy = (sh - bp) / 2;

    SDL_SetRenderDrawColor(renderer, BORDER.r, BORDER.g, BORDER.b, 255);
    for (int i = 0; i <= boardSize; i++)
    {
        SDL_RenderDrawLine(renderer, sx, sy + i * cs, sx + bp, sy + i * cs);
        SDL_RenderDrawLine(renderer, sx + i * cs, sy, sx + i * cs, sy + bp);
    }
    for (int i = 0; i < boardSize; i++)
    {
        drawTxt(std::to_string(i), sx - 22, sy + i * cs + cs / 2 - 7, GRAY, fSmall);
        drawTxt(std::to_string(i), sx + i * cs + cs / 2 - 5, sy - 20, GRAY, fSmall);
    }
    int mg = cs / 5;
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            int cx = sx + j * cs + cs / 2;
            int cy = sy + i * cs + cs / 2;
            int r = cs / 2 - mg;
            if (src[i][j] == 'X')
            {
                SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, 255);
                for (int t = -2; t <= 2; t++)
                {
                    SDL_RenderDrawLine(renderer, cx - r + t, cy - r, cx + r + t, cy + r);
                    SDL_RenderDrawLine(renderer, cx + r + t, cy - r, cx - r + t, cy + r);
                }
            }
            else if (src[i][j] == 'O')
            {
                SDL_SetRenderDrawColor(renderer, BLUE.r, BLUE.g, BLUE.b, 255);
                for (int deg = 0; deg < 360; deg++)
                {
                    float a1 = deg * (float)M_PI / 180.f;
                    float a2 = (deg + 1) * (float)M_PI / 180.f;
                    SDL_RenderDrawLine(renderer,
                                       cx + (int)(r * cos(a1)), cy + (int)(r * sin(a1)),
                                       cx + (int)(r * cos(a2)), cy + (int)(r * sin(a2)));
                }
            }
        }
    }
}

void SDLRenderer::displayBoard(const char board[][BOARD_N_MAX], int ctx)
{
    this->size = ctx;
    if (this->size > 0)
        boardSize = this->size;
    if (board)
        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
                this->board[i][j] = board[i][j];
    const char (*src)[BOARD_N_MAX] = board ? board : this->board;
    drawGridBg();
    drawBg(80);
    int pad = 60, bp = std::min(sw, sh) - pad * 2;
    int sx = (sw - bp) / 2, sy = (sh - bp) / 2;
    drawRect(sx, sy, bp, bp, {0, 0, 0, 160}, true);
    renderBoard(src);
    rframe();
}

void SDLRenderer::showPlayer(const int player, const bool is_bot)
{
    curPlayer = player;
    displayBoard(nullptr, 0);
    SDL_Color col = (player == 0) ? RED : BLUE;
    drawRect(15, 15, 30, 30, col, true);
    std::string lbl = is_bot
                          ? std::format("Bot {} ({})", player + 1, player == 0 ? "X" : "O")
                          : std::format("Player {} ({})", player + 1, player == 0 ? "X" : "O");
    drawTxt(lbl, 55, 20, WHITE, fMedium);
    rframe();
}

void SDLRenderer::showMove(const int row, const int col)
{
    if (audio)
        audio->playClick(curPlayer);
    char sym = (curPlayer == 0) ? 'X' : 'O';
    animatePlacePiece(row, col, sym);
}

void SDLRenderer::showInvalidMove()
{
    for (int i = 0; i < 3; i++)
    {
        drawRect(0, 0, sw, sh, {200, 0, 0, 60}, true);
        rframe();
        SDL_Delay(60);
        clearScreen();
        rframe();
        SDL_Delay(60);
    }
    drawTxtC("INVALID MOVE!", sw / 2, sh / 2, RED, fMedium);
    rframe();
    SDL_Delay(600);
}

void SDLRenderer::showResult(const int winner, const bool is_bot, const WinLine *wl)
{
    if (audio)
        audio->playWin();
    animateWinLine(wl);
    SDL_Color pc = (winner == DRAW_RESULT) ? WHITE : (winner == 0 ? RED : BLUE);
    spawnParticles(sw / 2, sh / 2, pc, 80);
    for (int f = 0; f < 40; f++)
    {
        drawGridBg();
        drawParticles();
        rframe();
        SDL_Delay(16);
    }

    drawRect(0, 0, sw, sh, {0, 0, 0, 170}, true);

    int cx = sw / 2, py = sh / 3;
    drawRect(cx - 240, py - 20, 480, 220, {40, 40, 60, 255}, true);
    drawRect(cx - 240, py - 20, 480, 220, BORDER, false);

    if (winner == DRAW_RESULT)
    {
        drawTxtC("DRAW!", cx, py + 20, WHITE, fLarge);
        drawTxtC("No winner this time", cx, py + 75, GRAY, fMedium);
    }
    else if (is_bot)
    {
        SDL_Color col = (winner == 0) ? RED : BLUE;
        drawTxtC(std::format("BOT {} WINS!", winner + 1), cx, py + 20, col, fLarge);
    }
    else
    {
        SDL_Color col = (winner == 0) ? RED : BLUE;
        drawTxtC("CONGRATULATIONS!", cx, py + 10, YELLOW, fMedium);
        drawTxtC(std::format("PLAYER {} WINS!", winner + 1), cx, py + 55, col, fLarge);
        drawTxtC("Well played!", cx, py + 110, GREEN, fMedium);
    }

    drawTxtC("Press any key to continue", cx, py + 165, GRAY, fSmall);
    rframe();
    SDL_Event e;
    while (SDL_WaitEvent(&e))
    {
        if (e.type == SDL_QUIT)
            throw QuitException();
        if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
            break;
    }
}

void SDLRenderer::printResult(const GameResult &gr)
{
    if (gr.winner == DRAW_RESULT)
        std::cout << "DRAW\n";
    else
        std::cout << std::format("WINNER: Player {} | TURNS: {}\n", gr.winner + 1, gr.turns);
}