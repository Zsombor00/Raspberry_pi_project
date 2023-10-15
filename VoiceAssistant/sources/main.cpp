#include "libraries.hpp"

#include "AudioAPIHandler.hpp"
#include "AudioRecorder.hpp"
#include "AudioReader.hpp"

void interactWithUser(
    AudioRecorder &audioRecorder,
    AudioAPIHandler &audioAPIHandler,
    AudioReader &audioReader,
    const std::filesystem::path &welcomeFilePath,
    const std::filesystem::path &inputFilePath,
    const std::filesystem::path &outputFilePath,
    const std::string &apiUrl
) {
    audioReader.play(welcomeFilePath);

    // std::cout << "Press Enter to start recording." << std::endl;
    // std::cin.get();
    audioRecorder.startRecording();

    std::cout << "Recording... Press Enter to stop." << std::endl;
    std::cin.get();
    audioRecorder.stopRecording();

    // std::cout << "Saving to " << inputFilePath << "..." << std::endl;
    audioRecorder.save(inputFilePath);

    // std::cout << "Sending " << inputFilePath << " to the API " << apiUrl << "..." << std::endl;
    // std::cout << "And saving the result to " << outputFilePath << "..." << std::endl;
    audioAPIHandler.uploadFile(inputFilePath, outputFilePath);

    // std::cout << "Press Enter to play " << outputFilePath << std::endl;
    // std::cin.get();
    audioReader.play(outputFilePath);
}

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
