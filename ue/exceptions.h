#pragma once


#include <string>
#include <stdexcept>


// Macro for creating runtime error exception types.
// Use these exceptions for problems that occur out of the programmer's control.
#define UE_NEW_RUNTIME_EXCEPTION_TYPE(name, default_msg) \
    class name: public std::runtime_error { \
        public: \
            name(const std::string& msg): \
                std::runtime_error(msg) \
            { \
            } \
            \
            name(): \
                std::runtime_error(default_msg) \
            { \
            } \
    };


// Macro for creating logic error exception types.
// Use these exceptions for problems that occur due to programmer error.
#define UE_NEW_LOGIC_EXCEPTION_TYPE(name, default_msg) \
    class name: public std::logic_error { \
        public: \
            name(const std::string& msg): \
                std::logic_error(msg) \
            { \
            } \
            \
            name(): \
                std::logic_error(default_msg) \
            { \
            } \
    };


namespace ue {
    const std::string sdl_error() {
        return "(SDL_GetError(): " + std::string{SDL_GetError()} + ")";
    }

    const std::string img_error() {
        return "(IMG_GetError(): " + std::string{IMG_GetError()} + ")";
    }

    const std::string mix_error() {
        return "(Mix_GetError(): " + std::string{Mix_GetError()} + ")";
    }

    const std::string ttf_error() {
        return "(TTF_GetError(): " + std::string{TTF_GetError()} + ")";
    }

    const std::string net_error() {
        return "(SDLNet_GetError(): " + std::string{SDLNet_GetError()} + ")";
    }
}


// Defining exception types.
namespace ue {
    // From memory.
    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorCreateSurface,
        "An error occurred when trying to create a surface! " + ue::sdl_error()
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorCreateFontFile,
        "An error occurred when trying to create a fontfile! " + ue::sdl_error()
    );


    // From file.
    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorCreateSurfaceFromFile,
        "An error occurred when trying to load a surface! " + ue::sdl_error()
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorCreateFontFileFromFile,
        "An error occurred when trying to load a fontfile! " + ue::sdl_error()
    );
}


#undef UE_NEW_RUNTIME_EXCEPTION_TYPE
#undef UE_NEW_LOGIC_EXCEPTION_TYPE