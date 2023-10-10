# CMake version
cmake_minimum_required(VERSION 3.5)

# Project name
project(VoiceAssistant LANGUAGES CXX)

# C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Sources
file(GLOB_RECURSE SOURCES sources/*.cpp     sources/*/*.cpp     sources/*/*/*.cpp)
file(GLOB_RECURSE HEADERS includes/*.hpp    includes/*/*.hpp    includes/*/*/*.hpp)

# Executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE portaudio sndfile mp3lame SDL2 SDL2_mixer)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE includes)

# Libraries
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
