// Wrapper for SDL, saves us having to include
// all the files manually everywhere.
#pragma once


extern "C" {
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_mixer.h>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_net.h>
}