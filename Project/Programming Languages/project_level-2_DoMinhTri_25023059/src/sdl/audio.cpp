#include "audio.h"

#include "../utils/logger.h"

const std::array<std::string, Audio::OST_COUNT> Audio::OST_NAMES = {
    "Main OST 1", "Main OST 2", "Main OST 3", "Main OST 4", "Main OST 5"};

const std::array<std::string, Audio::OST_COUNT> Audio::OST_FILES = {
    "assets/sounds/ost1.mp3",
    "assets/sounds/ost2.mp3",
    "assets/sounds/ost3.mp3",
    "assets/sounds/ost4.mp3",
    "assets/sounds/ost5.mp3"};

Audio::Audio() {}

Audio::~Audio() { close(); }

void Audio::init()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        Logger::log("SDL_mixer init failed", Logger::Level::WARNING);
        return;
    }

    clickSfx[0] = Mix_LoadWAV("assets/sounds/click_p1.wav");
    clickSfx[1] = Mix_LoadWAV("assets/sounds/click_p2.wav");
    winSfx = Mix_LoadWAV("assets/sounds/win.wav");

    if (!clickSfx[0] || !clickSfx[1])
        Logger::log("Click SFX load failed", Logger::Level::WARNING);

    playOst(0);
    Logger::log("Audio initialized", Logger::Level::INFO);
}

void Audio::close()
{
    Mix_HaltMusic();
    if (music)
    {
        Mix_FreeMusic(music);
        music = nullptr;
    }
    for (auto &sfx : clickSfx)
        if (sfx)
        {
            Mix_FreeChunk(sfx);
            sfx = nullptr;
        }
    if (winSfx)
    {
        Mix_FreeChunk(winSfx);
        winSfx = nullptr;
    }
    Mix_CloseAudio();
}

void Audio::playOst(int idx)
{
    if (idx < 0 || idx >= OST_COUNT)
        return;
    curOst = idx;
    if (music)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = nullptr;
    }
    music = Mix_LoadMUS(OST_FILES[idx].c_str());
    if (!music)
    {
        Logger::log("OST load failed", Logger::Level::WARNING);
        return;
    }
    Mix_VolumeMusic(volume * MIX_MAX_VOLUME / 100);
    Mix_PlayMusic(music, -1);
}

void Audio::nextOst() { playOst((curOst + 1) % OST_COUNT); }
void Audio::prevOst() { playOst((curOst - 1 + OST_COUNT) % OST_COUNT); }

void Audio::playClick(int player)
{
    int idx = (player == 0) ? 0 : 1;
    if (clickSfx[idx])
        Mix_PlayChannel(-1, clickSfx[idx], 0);
}

void Audio::playWin()
{
    Mix_HaltMusic();
    if (winSfx)
        Mix_PlayChannel(-1, winSfx, 0);
}

void Audio::setVolume(int vol)
{
    volume = std::max(0, std::min(100, vol));
    Mix_VolumeMusic(volume * MIX_MAX_VOLUME / 100);
}

std::string Audio::getCurOstName() const
{
    return OST_NAMES[curOst];
}
