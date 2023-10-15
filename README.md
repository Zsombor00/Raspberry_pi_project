# Voice Assistant

This project, developed as part of the Advanced Computer Science Studies in Sweden course at Uppsala University (Group 18), entails the creation of an intelligent voice assistant based on the ChatGPT platform. The members of Group 18 include Victor Delattre, Alexandru Gurita, Zsombor Kotan, and Zuwei Ye.

## Project Overview

In an endeavor to design and develop a portable and energy-sustainable voice assistant, we elected the Raspberry Pi Model 3B as our platform of choice, given its suitability for embedded systems. The core functionality of our Voice Assistant pivots on the interaction between the user and the device, wherein the user's voice input is processed and responded to via ChatGPT's voice output. This interaction is facilitated through a structured system comprising the Raspberry Pi, an intermediary Node server, and cloud computing technologies.

The Raspberry Pi, programmed using C++, handles sound recording, data transmission to the remote server, receipt of responses, and the rendering of audio output. On the other hand, the Node server, tasked with computing duties, orchestrates API calls to external servers, data assembly, and the transmission of structured data back to the Raspberry Pi. By leveraging mature cloud computing technologies, specifically ChatGPT Whisper, we've ensured a lightweight design and implementation, thereby optimizing the transcription and synthesis of voice data.

This repository hosts the core C++ code vital for the operation of the Voice Assistant system on the Raspberry Pi.

## Getting Started

### Dependencies

* Ensure you have Standard C++20 installed on your machine.

* Install the necessary third-party libraries by executing the following command:
```bash
sudo apt-get update -y && sudo apt-get upgrade -y && sudo apt-get install libcurl4-openssl-dev libmp3lame-dev portaudio19-dev libsdl2-dev libsdl2-mixer-dev libsndfile1-dev
```

### Installing
```bash
git clone https://github.com/Zsombor00/Raspberry_pi_project.git
cd ./Raspberry_pi_project/VoiceAssistant/
bash ./scripts/launch_unix.sh
```

### Executing program

* You can launch the program via the terminal using the following command:
```bash
./build/VoiceAssistant
```

* Alternatively, create a systemd service to initiate the program on system startup.

## Contributors

* Victor Delattre [@VictorDelattre](https://github.com/VictorDelattre)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
