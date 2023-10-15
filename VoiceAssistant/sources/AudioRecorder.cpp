/*!
 * \file AudioRecorder.cpp
 * \brief Implementation of the AudioRecorder class.
 */

#include "AudioRecorder.hpp"

/*!
 * \brief Constructs an AudioRecorder.
 * \param sampleRate The sample rate in Hertz.
 * \param numChannels The number of audio channels.
 * \param framesPerBuffer The number of frames per buffer.
 */
AudioRecorder::AudioRecorder(int sampleRate, int numChannels, int framesPerBuffer)
    : m_sampleRate{sampleRate}
    , m_numChannels{numChannels}
    , m_framesPerBuffer{framesPerBuffer}
    , m_state{State::Idle}
{
    auto err = Pa_Initialize();
    if (err != paNoError) {
        throw std::runtime_error(fmt::format("Failed to initialize PortAudio: {}", Pa_GetErrorText(err)));
    }
}

/*!
 * \brief Destructs the AudioRecorder, ensuring recording is stopped.
 */
AudioRecorder::~AudioRecorder()
{
    if (isRecording()) {
        stopRecording();
    }
    Pa_Terminate();
}

/*!
 * \brief Starts recording audio.
 *
 * If already recording, this function returns early.
 * Otherwise, it initializes the audio stream and spawns the recording thread.
 */
void AudioRecorder::startRecording()
{
    if (isRecording()) {
        return;
    }

    m_state = State::Recording;
    m_samples.clear();

    auto err = Pa_OpenDefaultStream(&m_stream, m_numChannels, 0, paInt16, m_sampleRate, m_framesPerBuffer, nullptr, nullptr);
    if (err != paNoError) {
        throw std::runtime_error(fmt::format("Failed to open default audio stream: {}", Pa_GetErrorText(err)));
    }

    err = Pa_StartStream(m_stream);
    if (err != paNoError) {
        throw std::runtime_error(fmt::format("Failed to start audio stream: {}", Pa_GetErrorText(err)));
    }

    m_recordingThread = std::jthread(&AudioRecorder::recordingLoop, this);
}

/*!
 * \brief Stops recording audio.
 *
 * If not currently recording, this function throws a runtime error.
 * Otherwise, it stops and closes the audio stream, and joins the recording thread.
 */
void AudioRecorder::stopRecording()
{
    if (!isRecording()) {
        throw std::runtime_error("Not currently recording!");
    }

    m_state = State::Idle;

    if (m_recordingThread.joinable()) {
        m_recordingThread.join();
    }

    auto err = Pa_StopStream(m_stream);
    if (err != paNoError) {
        throw std::runtime_error(fmt::format("Failed to stop audio stream: {}", Pa_GetErrorText(err)));
    }

    err = Pa_CloseStream(m_stream);
    if (err != paNoError) {
        throw std::runtime_error(fmt::format("Failed to close audio stream: {}", Pa_GetErrorText(err)));
    }
}

/*!
 * \brief Saves the recorded audio to a file.
 * \param filePath The name of the file to save the audio to.
 */
void AudioRecorder::save(std::filesystem::path const& filePath) const
{
    FileFormat format = determineFileFormat(filePath);
    switch (format) {
    case FileFormat::WAV:
    case FileFormat::FLAC:
    case FileFormat::OGG:
        saveAsWavFlacOgg(filePath, format);
        break;
    case FileFormat::MP3:
        saveAsMp3(filePath);
        break;
    case FileFormat::UNKNOWN:
    default:
        throw std::runtime_error(fmt::format("Unsupported file format: {}", filePath.string()));
    }
}

/*!
 * \brief Checks if the AudioRecorder is currently recording.
 * \return True if recording, false otherwise.
 */
bool AudioRecorder::isRecording() const noexcept
{
    return m_state == State::Recording;
}

/*!
 * \brief The recording loop, reads audio data from the audio stream and saves it to a buffer.
 *
 * This function is intended to be run in a separate thread, and continues recording audio
 * data until recording is stopped.
 */
void AudioRecorder::recordingLoop()
{
    while (isRecording()) {
        std::vector<short> buffer(m_framesPerBuffer * m_numChannels);
        auto read = Pa_ReadStream(m_stream, buffer.data(), m_framesPerBuffer);
        if (read != paNoError) {
            throw std::runtime_error(fmt::format("Error during audio stream read: {}", Pa_GetErrorText(read)));
        }

        m_samples.insert(m_samples.end(), buffer.begin(), buffer.end());
    }
}

/*!
 * \brief Determines the file format based on the file extension.
 * \param filePath The name of the file.
 * \return The determined FileFormat.
 */
[[nodiscard]] AudioRecorder::FileFormat AudioRecorder::determineFileFormat(std::filesystem::path const& filePath) const
{
    std::string extension = std::filesystem::path(filePath).extension().string();
    if (extension == ".wav") return FileFormat::WAV;
    if (extension == ".flac") return FileFormat::FLAC;
    if (extension == ".ogg") return FileFormat::OGG;
    if (extension == ".mp3") return FileFormat::MP3;
    return FileFormat::UNKNOWN;
}

/*!
 * \brief Saves the recorded audio to a file in WAV, FLAC, or OGG format.
 * \param filePath The name of the file to save the audio to.
 * \param format The file format to save the audio in.
 */
void AudioRecorder::saveAsWavFlacOgg(std::filesystem::path const& filePath, FileFormat format) const
{
    // Mapping file extensions to SFML format constants
    std::map<std::string, int> formatMap = {
        { ".wav", SF_FORMAT_WAV },
        { ".flac", SF_FORMAT_FLAC },
        { ".ogg", SF_FORMAT_OGG },
    };

    std::string extension = filePath.extension().string();
    auto formatIter = formatMap.find(extension);
    if (formatIter == formatMap.end()) {
        throw std::runtime_error("Unsupported file format");
    }

    SF_INFO sfInfo;
    sfInfo.samplerate = m_sampleRate;
    sfInfo.channels = m_numChannels;
    sfInfo.format = formatIter->second | SF_FORMAT_PCM_16;

    std::unique_ptr<SNDFILE, decltype(&sf_close)> outfile(sf_open(filePath.c_str(), SFM_WRITE, &sfInfo), &sf_close);
    if (!outfile) {
        throw std::runtime_error(fmt::format("Failed to open file for writing: {}", filePath.string()));
    }

    sf_write_short(outfile.get(), m_samples.data(), m_samples.size());
}

/*!
 * \brief Saves the recorded audio to a file in MP3 format.
 * \param filePath The name of the file to save the audio to.
 */
void AudioRecorder::saveAsMp3(std::filesystem::path const& filePath) const
{
    lame_global_flags *gfp = lame_init();
    if (!gfp) {
        throw std::runtime_error("Failed to initialize LAME encoder");
    }

    lame_set_in_samplerate(gfp, m_sampleRate);  // Use class member for sample rate
    lame_set_num_channels(gfp, m_numChannels);  // Set number of channels
    lame_set_VBR(gfp, vbr_default);
    lame_init_params(gfp);

    // Calculate the total number of samples for a single channel
    int numSamples = m_samples.size() / m_numChannels;

    // Allocate memory for MP3 buffer based on LAME's maximum output size formula
    const int MP3_BUFFER_SIZE = 1.25 * numSamples + 7200;
    std::unique_ptr<unsigned char[]> mp3Buffer(new unsigned char[MP3_BUFFER_SIZE]);

    int mp3Size;
    if (m_numChannels == 2) {  // Stereo
        std::vector<short> leftBuffer(numSamples);
        std::vector<short> rightBuffer(numSamples);
        for (int i = 0, j = 0; i < m_samples.size(); i += 2, ++j) {
            leftBuffer[j] = m_samples[i];
            rightBuffer[j] = m_samples[i + 1];
        }
        mp3Size = lame_encode_buffer(gfp, leftBuffer.data(), rightBuffer.data(), numSamples, mp3Buffer.get(), MP3_BUFFER_SIZE);
    } else {  // Mono
        mp3Size = lame_encode_buffer(gfp, m_samples.data(), nullptr, numSamples, mp3Buffer.get(), MP3_BUFFER_SIZE);
    }

    if (mp3Size < 0) {
        lame_close(gfp);
        throw std::runtime_error("Error encoding MP3 data");
    }

    // Handle the flush of the encoder to get any remaining data
    int flushSize = lame_encode_flush(gfp, mp3Buffer.get() + mp3Size, MP3_BUFFER_SIZE - mp3Size);
    if (flushSize < 0) {
        lame_close(gfp);
        throw std::runtime_error("Error flushing MP3 encoder");
    }
    mp3Size += flushSize;

    FILE *mp3File = fopen(filePath.c_str(), "wb");
    if (!mp3File) {
        lame_close(gfp);
        throw std::runtime_error("Error opening MP3 file for writing");
    }

    fwrite(mp3Buffer.get(), 1, mp3Size, mp3File);
    fclose(mp3File);

    lame_close(gfp);
}
