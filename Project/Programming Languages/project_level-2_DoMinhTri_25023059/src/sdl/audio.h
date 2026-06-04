#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>
#include <array>

class Audio
{
private:
    Mix_Music *music = nullptr;
    std::array<Mix_Chunk *, 2> clickSfx = {nullptr, nullptr};
    Mix_Chunk *winSfx = nullptr;

    int curOst = 0;
    int volume = 80;

    static constexpr int OST_COUNT = 5;
    static const std::array<std::string, OST_COUNT> OST_NAMES;
    static const std::array<std::string, OST_COUNT> OST_FILES;

public:
    Audio();
    ~Audio();

    void init();
    void close();

    void playOst(int idx);
    void nextOst();
    void prevOst();

    void playClick(int player);
    void playWin();

    void setVolume(int vol);
    int getVolume() const { return volume; }
    int getCurOst() const { return curOst; }
    std::string getCurOstName() const;
};
