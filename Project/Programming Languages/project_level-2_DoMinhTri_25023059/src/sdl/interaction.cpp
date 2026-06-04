#include "interaction.h"
#include "renderer.h"

#include <algorithm>
#include <format>

#include "../utils/logger.h"

SDLInteraction::SDLInteraction() {}
SDLInteraction::~SDLInteraction() {}

void SDLInteraction::setRenderer(SDLRenderer *r) { renderer = r; }

void SDLInteraction::waitForQuit(SDL_Event &e)
{
    if (e.type == SDL_QUIT)
        throw QuitException();
}

void SDLInteraction::init(const RunConfig &config)
{
    sw = config.screenWidth;
    sh = config.screenHeight;
    boardPad = config.boardPadding;
    Logger::log("SDLInteraction initialized", Logger::Level::INFO);
}

void SDLInteraction::pause(int timeout)
{
    if (timeout > 0)
    {
        SDL_Delay(timeout);
        return;
    }
    SDL_Event e;
    while (true)
    {
        if (SDL_WaitEvent(&e))
        {
            waitForQuit(e);
            if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
                return;
        }
    }
}

void SDLInteraction::handleMainMenu()
{
    if (!renderer)
        return;

    int sel = 0;
    renderer->showMainMenu(sel);

    int scrW = renderer->getScreenWidth();
    int scrH = renderer->getScreenHeight();

    SDL_Event e;
    while (true)
    {
        while (SDL_PollEvent(&e))
        {
            waitForQuit(e);

            if (e.type == SDL_MOUSEMOTION)
            {
                int mx = e.motion.x, my = e.motion.y;
                int cx = sw / 2;
                int startY = 280;
                int bw = 300, bh = 46;

                for (int i = 0; i < 4; i++)
                {
                    int by = startY + i * 65;
                    if (mx >= cx - bw / 2 && mx <= cx + bw / 2 && my >= by && my <= by + bh)
                    {
                        if (sel != i)
                        {
                            sel = i;
                            renderer->showMainMenu(sel);
                        }
                        break;
                    }
                }
            }

            if (e.type == SDL_KEYDOWN)
            {
                SDL_Keycode k = e.key.keysym.sym;
                if (k == SDLK_UP)
                {
                    sel = (sel + 3) % 4;
                    renderer->showMainMenu(sel);
                }
                if (k == SDLK_DOWN)
                {
                    sel = (sel + 1) % 4;
                    renderer->showMainMenu(sel);
                }
                if (k == SDLK_RETURN || k == SDLK_KP_ENTER)
                {
                    if (sel == 0)
                    {
                        renderer->setCurScreen(MenuScreen::GAME);
                        return;
                    }
                    if (sel == 1)
                    {
                        handleSettings();
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                        renderer->showMainMenu(sel);
                    }
                    if (sel == 3)
                    {
                        throw QuitException();
                    }
                    if (sel == 2)
                    {
                        renderer->showAbout();
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                        SDL_Event ae;
                        while (SDL_WaitEvent(&ae))
                        {
                            waitForQuit(ae);
                            if (ae.type == SDL_KEYDOWN || ae.type == SDL_MOUSEBUTTONDOWN)
                                break;
                        }
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                        renderer->showMainMenu(sel);
                    }
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x, my = e.button.y;
                int cx = sw / 2;
                int startY = 280;
                int bw = 300, bh = 46;

                for (int i = 0; i < 4; i++)
                {
                    int by = startY + i * 65;
                    if (mx >= cx - bw / 2 && mx <= cx + bw / 2 && my >= by && my <= by + bh)
                    {
                        sel = i;
                        renderer->showMainMenu(sel);
                        SDL_Delay(80);
                        if (sel == 0)
                        {
                            renderer->setCurScreen(MenuScreen::GAME);
                            return;
                        }
                        if (sel == 1)
                        {
                            handleSettings();
                            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                            renderer->showMainMenu(sel);
                        }
                        if (sel == 3)
                        {
                            throw QuitException();
                        }
                        if (sel == 2)
                        {
                            renderer->showAbout();
                            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                            SDL_Event ae;
                            while (SDL_WaitEvent(&ae))
                            {
                                waitForQuit(ae);
                                if (ae.type == SDL_KEYDOWN || ae.type == SDL_MOUSEBUTTONDOWN)
                                    break;
                            }
                            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                            renderer->showMainMenu(sel);
                        }
                    }
                }
            }
        }
        SDL_Delay(16);
    }
}
void SDLInteraction::handleSettings()
{
    if (!renderer)
        return;
    Audio *audio = renderer->getAudio();

    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    renderer->showSettings();

    int scrW = renderer->getScreenWidth();
    int scrH = renderer->getScreenHeight();

    SDL_Event e;
    while (true)
    {
        while (SDL_PollEvent(&e))
        {
            waitForQuit(e);
            if (e.type == SDL_KEYDOWN)
            {
                SDL_Keycode k = e.key.keysym.sym;
                if (k == SDLK_ESCAPE)
                    return;

                if (audio)
                {
                    if (k == SDLK_LEFT)
                    {
                        if (renderer->getUseBgImage())
                        {
                            renderer->prevBg();
                        }
                        else
                        {
                            audio->prevOst();
                        }
                        renderer->showSettings();
                    }
                    if (k == SDLK_RIGHT)
                    {
                        if (renderer->getUseBgImage())
                        {
                            renderer->nextBg();
                        }
                        else
                        {
                            audio->nextOst();
                        }
                        renderer->showSettings();
                    }
                    if (k == SDLK_UP)
                    {
                        audio->setVolume(audio->getVolume() + 5);
                        renderer->showSettings();
                    }
                    if (k == SDLK_DOWN)
                    {
                        audio->setVolume(audio->getVolume() - 5);
                        renderer->showSettings();
                    }
                }

                if (k == SDLK_b)
                {
                    renderer->toggleBg();
                    renderer->showSettings();
                }

                if (k == SDLK_RETURN || k == SDLK_KP_ENTER)
                    return;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x, my = e.button.y;

                int bx = scrW / 2 - 100, by = scrH - 100, bw = 200, bh = 48;
                if (mx >= bx && mx <= bx + bw && my >= by && my <= by + bh)
                    return;

                int px = scrW / 2 - 100, py = scrH - 290, pw = 200, ph = 112;
                if (mx >= px && mx <= px + pw && my >= py && my <= py + ph)
                {
                    if (mx < scrW / 2)
                    {
                        renderer->prevBg();
                    }
                    else
                    {
                        renderer->nextBg();
                    }
                    renderer->showSettings();
                }

                if (mx >= scrW / 2 - 150 && mx <= scrW / 2 + 150 && my >= py + ph + 6 && my <= py + ph + 26)
                {
                    renderer->toggleBg();
                    renderer->showSettings();
                }

                if (audio)
                {
                    if (my >= 110 && my <= 170)
                    {
                        if (mx >= scrW / 2 - 200 && mx <= scrW / 2 - 60)
                        {
                            audio->prevOst();
                            renderer->showSettings();
                        }
                        else if (mx >= scrW / 2 + 60 && mx <= scrW / 2 + 200)
                        {
                            audio->nextOst();
                            renderer->showSettings();
                        }
                    }

                    int barX = scrW / 2 - 150, barY = 250, barW = 300, barH = 16;
                    if (mx >= barX && mx <= barX + barW && my >= barY && my <= barY + barH)
                    {
                        int vol = (mx - barX) * 100 / barW;
                        audio->setVolume(vol);
                        renderer->showSettings();
                    }
                }
            }
        }
        SDL_Delay(16);
    }
}

static bool inputLoop(SDLRenderer *rdr, std::string &buf, int maxLen = 2)
{
    SDL_Event e;
    while (true)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                throw QuitException();
            if (e.type == SDL_KEYDOWN)
            {
                SDL_Keycode k = e.key.keysym.sym;
                if (k >= SDLK_0 && k <= SDLK_9 && (int)buf.size() < maxLen)
                {
                    buf += (char)k;
                    if (rdr)
                        rdr->setTempInput(buf);
                }
                else if ((k == SDLK_RETURN || k == SDLK_KP_ENTER) && !buf.empty())
                {
                    return true;
                }
                else if (k == SDLK_BACKSPACE && !buf.empty())
                {
                    buf.pop_back();
                    if (rdr)
                        rdr->setTempInput(buf);
                }
                else if (k == SDLK_ESCAPE)
                {
                    return false;
                }
            }
        }
        SDL_Delay(16);
    }
}

bool SDLInteraction::selectSize(int *size)
{
    if (!renderer)
        return false;
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    curInput.clear();
    renderer->setTempInput("");
    renderer->setInputRange(BOARD_N_MIN, BOARD_N_MAX);
    renderer->showSelectMenu(SelectType::SIZE_UI, 0);

    while (true)
    {
        if (!inputLoop(renderer, curInput))
            return false;
        int v = std::stoi(curInput);
        if (v >= BOARD_N_MIN && v <= BOARD_N_MAX)
        {
            *size = v;
            boardSize = v;
            renderer->setBoardSize(v);
            return true;
        }
        renderer->showInvalidSelect(SelectType::SIZE_UI, 0);
        curInput.clear();
    }
}

bool SDLInteraction::selectGoal(int *goal, const int size)
{
    if (!renderer)
        return false;
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    curInput.clear();
    int maxGoal = std::min(size, 5);
    renderer->setTempInput("");
    renderer->setInputRange(3, maxGoal);
    renderer->showSelectMenu(SelectType::GOAL_UI, size);

    while (true)
    {
        if (!inputLoop(renderer, curInput))
            return false;
        int v = std::stoi(curInput);
        if (v >= 3 && v <= maxGoal)
        {
            *goal = v;
            return true;
        }
        renderer->showInvalidSelect(SelectType::GOAL_UI, size);
        curInput.clear();
    }
}

bool SDLInteraction::selectGameMode(GameMode *mode)
{
    if (!renderer)
        return false;
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    curInput.clear();
    renderer->setTempInput("");
    renderer->setInputRange(1, 3);
    renderer->showSelectMenu(SelectType::GAME_MODE_UI, 0);

    while (true)
    {
        if (!inputLoop(renderer, curInput, 1))
            return false;
        int v = std::stoi(curInput);
        switch (v)
        {
        case 1:
            *mode = GameMode::PVP;
            return true;
        case 2:
            *mode = GameMode::PVE;
            return true;
        case 3:
            *mode = GameMode::EVE;
            return true;
        }
        renderer->showInvalidSelect(SelectType::GAME_MODE_UI, 0);
        curInput.clear();
    }
}

bool SDLInteraction::selectBotLevel(BotLevel *levels, const int index)
{
    if (!renderer || index < 0 || index > 1)
        return false;
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    curInput.clear();
    renderer->setTempInput("");
    renderer->setInputRange(1, 3);
    renderer->showSelectMenu(SelectType::MUL_BOT_LEVEL_UI, index);

    while (true)
    {
        if (!inputLoop(renderer, curInput, 1))
            return false;
        int v = std::stoi(curInput);
        switch (v)
        {
        case 1:
            levels[index] = BotLevel::EASY;
            return true;
        case 2:
            levels[index] = BotLevel::MEDIUM;
            return true;
        case 3:
            levels[index] = BotLevel::HARD;
            return true;
        }
        renderer->showInvalidSelect(SelectType::MUL_BOT_LEVEL_UI, index);
        curInput.clear();
    }
}

bool SDLInteraction::getPlayerMove(int *row, int *col)
{
    SDL_Event e;
    while (true)
    {
        if (renderer)
            renderer->displayBoard(nullptr, boardSize);
        while (SDL_PollEvent(&e))
        {
            waitForQuit(e);
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    renderer->setCurScreen(MenuScreen::MAIN);
                    throw QuitException();
                }
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                int mx = e.button.x, my = e.button.y;
                int pad = 60;
                int bp = std::min(sw, sh) - pad * 2;
                int cs = bp / boardSize;
                int sx = (sw - bp) / 2, sy = (sh - bp) / 2;
                if (mx < sx || my < sy || mx > sx + bp || my > sy + bp)
                    continue;
                int r = (my - sy) / cs;
                int c = (mx - sx) / cs;
                if (r < 0 || r >= boardSize || c < 0 || c >= boardSize)
                    continue;
                *row = r;
                *col = c;
                return true;
            }
        }
        SDL_Delay(16);
    }
}

void SDLInteraction::close()
{
    Logger::log("SDLInteraction closed", Logger::Level::INFO);
}