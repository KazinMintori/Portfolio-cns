#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <cstdlib>

#include "../game/interface/i_renderer.h"
#include "../game/logic.h"
#include "audio.h"

enum class MenuScreen
{
    MAIN,
    SETTINGS,
    ABOUT,
    GAME
};

class SDLRenderer : public I_Renderer
{
private:
    SDL_Texture *bgTexture = nullptr;
    SDL_Texture *logoTexture = nullptr;
    int curBgIdx = 0;
    std::vector<std::string> bgPaths;
    bool useBgImage = true;
    int size;
    char board[BOARD_N_MAX][BOARD_N_MAX];
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    TTF_Font *fLarge = nullptr;
    TTF_Font *fMedium = nullptr;
    TTF_Font *fSmall = nullptr;
    TTF_Font *fXLarge = nullptr;

    int sw = 800, sh = 800;
    int boardSize = BOARD_N_MAX;
    int curPlayer = 0;

    struct Star
    {
        float x, y, speed, brightness;
        int size;
    };
    std::vector<Star> stars;

    struct Particle
    {
        float x, y, vx, vy;
        SDL_Color color;
        int life, maxLife;
    };
    std::vector<Particle> particles;

    void loadBg(const std::string &path);
    void drawBg(Uint8 alpha = 180);
    void drawLogoXO();
    void initStars();
    void drawStarField();
    void drawGradientBg(SDL_Color top, SDL_Color bot);
    void drawGridBg();
    void spawnParticles(int cx, int cy, SDL_Color col, int count);
    void drawParticles();

    SelectType curMenuType = SelectType::TITLE_UI;
    int curCtx = NO_CONTEXT;

    std::string tmpInput;
    int inMin = 0, inMax = 100;

    MenuScreen curScreen = MenuScreen::MAIN;
    int mainMenuSel = 0;

    Audio *audio = nullptr;

    void drawRect(int x, int y, int w, int h, SDL_Color c, bool fill = true);
    void drawTxt(const std::string &s, int x, int y, SDL_Color c, TTF_Font *f);
    void drawTxtC(const std::string &s, int cx, int y, SDL_Color c, TTF_Font *f);
    void drawInputBox(int y);
    void renderBoard(const char src[][BOARD_N_MAX]);
    void drawButton(const std::string &label, int cx, int y, int w, int h,
                    bool selected, SDL_Color col = {60, 60, 80, 255});
    void animatePlacePiece(int row, int col, char symbol);
    void animateWinLine(const WinLine *wl);

public:
    bool getUseBgImage() const { return useBgImage; }
    SDLRenderer();
    ~SDLRenderer();
    void toggleBg()
    {
        useBgImage = !useBgImage;
    }
    void init(const RunConfig &config) override;
    void clearScreen() override;
    void showSelectMenu(SelectType t, int ctx = NO_CONTEXT) override;
    void showInvalidSelect(SelectType t, int ctx = NO_CONTEXT) override;
    void showValidSelect(SelectType t, int ctx = NO_CONTEXT) override;
    void displayBoard(const char board[][BOARD_N_MAX], int ctx) override;
    void showMove(const int row, const int col) override;
    void showInvalidMove() override;
    void showPlayer(const int player, const bool is_bot) override;
    void showResult(const int winner, const bool is_bot, const WinLine *winLine = nullptr) override;
    void printResult(const GameResult &gameResult) override;
    void close() override;

    void showMainMenu(int selected = 0);
    void showSettings();
    void showAbout();
    void nextBg();
    void prevBg();

    void setTempInput(const std::string &s);
    void setInputRange(int mn, int mx);
    void refreshMenu();
    void setBoardSize(int s) { boardSize = s; }
    int getBoardSize() const { return boardSize; }
    int getScreenWidth() const { return sw; }
    int getScreenHeight() const { return sh; }

    void setAudio(Audio *a) { audio = a; }
    void rframe();
    Audio *getAudio() { return audio; }
    MenuScreen getCurScreen() const { return curScreen; }
    void setCurScreen(MenuScreen s) { curScreen = s; }
};