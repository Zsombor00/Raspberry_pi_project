/*!
 * \file AudioAPIHandler.hpp
 * \brief Header file for the AudioAPIHandler class.
 */

#ifndef AUDIOAPIHANDLER_HPP
#define AUDIOAPIHANDLER_HPP
#pragma once

#include "libraries.hpp"

class AudioAPIHandler
{
public:
    explicit AudioAPIHandler();
    AudioAPIHandler(AudioAPIHandler const&) = delete;
    AudioAPIHandler(AudioAPIHandler &&) = delete;
    AudioAPIHandler & operator=(AudioAPIHandler const&) = delete;
    AudioAPIHandler & operator=(AudioAPIHandler &&) = delete;
    ~AudioAPIHandler() = default;

private:

};

#endif
