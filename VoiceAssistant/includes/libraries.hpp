// Third-party libraries headers
#include <curl/curl.h>
#include <lame/lame.h>
#include <portaudio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <sndfile.h>

// Standard library headers
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <thread>
#include <vector>

// namespace std {
//     template<>
//     struct formatter<std::filesystem::path> {
//         template<typename ParseContext>
//         constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }
//
//         template<typename FormatContext>
//         auto format(const std::filesystem::path& p, FormatContext& ctx) {
//             return std::formatter<std::string>().format(p.string(), ctx);
//         }
//     };
// }
