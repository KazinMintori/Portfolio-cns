#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "../game/interface/i_interaction.h"
#include "../game/setup.h"
#include "../utils/config.h"

class SDLRenderer;

class SDLInteraction : public I_Interaction
{
private:
    SDLRenderer *renderer = nullptr;
    int boardSize = BOARD_N_MAX;
    int sw = 800, sh = 800;
    int boardPad = 100;

    std::string curInput;

    void waitForQuit(SDL_Event &e);

public:
    SDLInteraction();
    ~SDLInteraction();

    void init(const RunConfig &config) override;
    void pause(int timeout = 0) override;
    bool selectSize(int *size) override;
    bool selectGoal(int *goal, const int size) override;
    bool selectGameMode(GameMode *mode) override;
    bool selectBotLevel(BotLevel *levels, const int index) override;
    bool getPlayerMove(int *row, int *col) override;
    void close() override;

    void setRenderer(SDLRenderer *r);
    void handleMainMenu();
    void handleSettings();
};
