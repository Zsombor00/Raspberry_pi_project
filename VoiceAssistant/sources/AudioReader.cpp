/*!
 * \file AudioReader.cpp
 * \brief Implementation of the AudioReader class.
 */

#include "AudioReader.hpp"

/*!
 * \brief Constructs an AudioReader.
 *
 * Initializes SDL2 audio subsystem and SDL_mixer library.
 * If initialization fails, an exception is thrown.
 */
AudioReader::AudioReader()
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        throw std::runtime_error(std::format("SDL2 error: {}", SDL_GetError()));
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Quit();
        throw std::runtime_error(std::format("SDL_mixer error: {}", Mix_GetError()));
    }
}

/*!
 * \brief Destructs the AudioReader.
 *
 * Closes the SDL_mixer library and shuts down the SDL2 audio subsystem.
 */
AudioReader::~AudioReader()
{
    Mix_Quit();
    SDL_Quit();
}

/*!
 * \brief Plays an audio file.
 * \param filePath The path of the audio file to play.
 *
 * Loads the audio file specified by filePath and plays it.
 * If there is an error loading the file or playing the audio, an exception is thrown.
 * This function blocks until the audio playback is complete.
 */
void AudioReader::play(std::filesystem::path const& filePath)
{
    MixMusicPtr music(Mix_LoadMUS(filePath.c_str()));
    if (!music) {
        throw std::runtime_error(std::format("SDL_mixer error: {}", Mix_GetError()));
    }

    if (Mix_PlayMusic(music.get(), 1) == -1) {
        throw std::runtime_error(std::format("SDL_mixer error: {}", Mix_GetError()));
    }

    while (Mix_PlayingMusic()) {
        SDL_Delay(100);  // consider a named constant for '100'
    }
}
