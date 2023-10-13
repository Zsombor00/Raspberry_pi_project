/*!
 * \file AudioAPIHandler.hpp
 * \brief Declaration of the AudioAPIHandler class.
 */

#ifndef AUDIOAPIHANDLER_HPP
#define AUDIOAPIHANDLER_HPP
#pragma once

#include "libraries.hpp"

/*!
 * \class AudioAPIHandler
 * \brief Handles interaction with a remote audio API using the cURL library.
 *
 * The AudioAPIHandler class provides methods to upload and download audio files
 * to and from a remote audio processing API. It utilizes the cURL library to
 * manage HTTP requests and responses.
 */
class AudioAPIHandler
{
public:
    explicit AudioAPIHandler(std::string const&);
    AudioAPIHandler(AudioAPIHandler const&) = delete;
    AudioAPIHandler(AudioAPIHandler &&) = delete;
    AudioAPIHandler & operator=(AudioAPIHandler const&) = delete;
    AudioAPIHandler & operator=(AudioAPIHandler &&) = delete;
    ~AudioAPIHandler();

    std::optional<std::string> uploadFile(std::filesystem::path const&, std::filesystem::path const&) const;

private:
    static size_t writeCallback(void*, size_t, size_t, void*);

    const std::string m_apiUrl;
    CURL *m_curl;
};

#endif
