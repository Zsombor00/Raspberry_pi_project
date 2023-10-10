/*!
 * \file AudioReader.hpp
 * \brief Header file for the AudioReader class.
 */

#ifndef AUDIOREADER_HPP
#define AUDIOREADER_HPP
#pragma once

#include "libraries.hpp"

/*!
 * \class AudioReader
 * \brief Handles audio playing using the SDL2 library.
 */
class AudioReader
{
public:
    explicit AudioReader();
    AudioReader(AudioReader const&) = delete;
    AudioReader(AudioReader &&) = delete;
    AudioReader & operator=(AudioReader const&) = delete;
    AudioReader & operator=(AudioReader &&) = delete;
    ~AudioReader();

    void play(std::string const&);

private:
    struct MixMusicDeleter {
        void operator()(Mix_Music *music) const { Mix_FreeMusic(music); }
    };

    using MixMusicPtr = std::unique_ptr<Mix_Music, MixMusicDeleter>;

};

#endif
