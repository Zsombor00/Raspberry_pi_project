/*!
 * \file main.cpp
 * \brief Main entry point for the audio interaction program.
 */

#include "libraries.hpp"

#include "AudioAPIHandler.hpp"
#include "AudioRecorder.hpp"
#include "AudioReader.hpp"

/*!
 * \brief Handles user interaction, audio recording, API communication, and audio playback.
 * \param audioRecorder An object to handle audio recording.
 * \param audioAPIHandler An object to handle communication with the audio processing API.
 * \param audioReader An object to handle audio playback.
 * \param welcomeFilePath The file path to the welcome audio file.
 * \param inputFilePath The file path to save the recorded audio.
 * \param outputFilePath The file path to save the API's audio response.
 * \param apiUrl The URL of the audio processing API.
 *
 * This function guides the user through the process of recording an audio snippet,
 * sends the recorded audio to a specified API, and plays back the received audio response.
 */
void interactWithUser(
    AudioRecorder &audioRecorder,
    AudioAPIHandler &audioAPIHandler,
    AudioReader &audioReader,
    const std::filesystem::path &welcomeFilePath,
    const std::filesystem::path &inputFilePath,
    const std::filesystem::path &outputFilePath,
    const std::string &apiUrl
) {
    std::cout << "Initial startup of the program... Press Enter to start interacting." << std::endl;
    std::cin.get();

    audioReader.play(welcomeFilePath);

    audioRecorder.startRecording();
    std::cout << "Recording... Press Enter to stop." << std::endl;
    std::cin.get();
    audioRecorder.stopRecording();
    audioRecorder.save(inputFilePath);

    std::cout << "Awaiting API's response..." << std::endl;
    audioAPIHandler.uploadFile(inputFilePath, outputFilePath);

    audioReader.play(outputFilePath);
}

/*!
 * \brief Entry point of the program.
 * \param argc The number of command line arguments.
 * \param argv The command line arguments.
 * \return 0 on success, or an error code on failure.
 *
 * This function initializes objects required for audio recording, API communication, and audio playback.
 * It then calls `interactWithUser` to guide the user through the audio recording and playback process.
 */
int main(int argc, char *argv[]) {
    constexpr std::string_view fileFormat = "mp3";
    const std::filesystem::path welcomeFilePath = std::string("./resources/sounds/startup_greeting.mp3");
    const std::filesystem::path inputFilePath = std::string("./resources/temp/input.") + fileFormat.data();
    const std::filesystem::path outputFilePath = std::string("./resources/temp/output.") + fileFormat.data();
    const std::string apiUrl = "https://advanced-computer-sciences-proj.vercel.app/api/assistant";

    try {
        AudioRecorder audioRecorder;
        AudioAPIHandler audioAPIHandler(apiUrl);
        AudioReader audioReader;

        interactWithUser(audioRecorder, audioAPIHandler, audioReader, welcomeFilePath, inputFilePath, outputFilePath, apiUrl);

    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
