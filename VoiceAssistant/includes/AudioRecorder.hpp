/*!
 * \file AudioRecorder.hpp
 * \brief Declaration of the AudioRecorder class.
 */

#ifndef AUDIORECORDER_HPP
#define AUDIORECORDER_HPP
#pragma once

#include "libraries.hpp"

/*!
 * \class AudioRecorder
 * \brief Handles audio recording using the PortAudio library.
 */
class AudioRecorder
{
public:
    explicit AudioRecorder(int = DEFAULT_SAMPLE_RATE, int = DEFAULT_NUM_CHANNELS, int = DEFAULT_FRAMES_PER_BUFFER);
    AudioRecorder(AudioRecorder const&) = delete;
    AudioRecorder(AudioRecorder &&) = delete;
    AudioRecorder & operator=(AudioRecorder const&) = delete;
    AudioRecorder & operator=(AudioRecorder &&) = delete;
    ~AudioRecorder();

    void startRecording();
    void stopRecording();
    void save(std::filesystem::path const&) const;

private:
    // Default settings for the audio capture
    inline static constexpr int DEFAULT_SAMPLE_RATE = 44100;
    inline static constexpr int DEFAULT_NUM_CHANNELS = 1;
    inline static constexpr int DEFAULT_FRAMES_PER_BUFFER = 512;

    enum class State {
        Idle,
        Recording
    };

    enum class FileFormat {
        UNKNOWN,
        WAV,
        FLAC,
        OGG,
        MP3
    };

    bool isRecording() const noexcept;
    void recordingLoop();
    [[nodiscard]] FileFormat determineFileFormat(std::filesystem::path const&) const;
    void saveAsWavFlacOgg(std::filesystem::path const&, FileFormat) const;
    void saveAsMp3(std::filesystem::path const&) const;

    int m_sampleRate;
    int m_numChannels;
    int m_framesPerBuffer;
    PaStream *m_stream;
    State m_state;
    std::vector<short> m_samples;
    std::jthread m_recordingThread;
};

#endif
