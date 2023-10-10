#include "libraries.hpp"

#include "AudioRecorder.hpp"
#include "AudioReader.hpp"

int main(int argc, char *argv[])
{
    // Ask for the format here if needed (replace mp3 with anything)
    std::string fileFormat("mp3");
    std::string fileName("./resources/temp/output." + fileFormat);

    try {
        AudioRecorder audioRecorder;
        AudioReader audioReader;

        std::cout << "Press Enter to start recording." << std::endl;
        std::cin.get();
        audioRecorder.startRecording();

        std::cout << "Recording... Press Enter to stop." << std::endl;
        std::cin.get();
        audioRecorder.stopRecording();

        std::cout << "Saving to " << fileName << "..." << std::endl;
        std::cout << std::endl;
        audioRecorder.save(fileName);

        std::cout << "Press Enter to play " << fileName << std::endl;
        std::cin.get();
        audioReader.play(fileName);

        // Delete the sound files here if needed
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
