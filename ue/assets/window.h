// WINDOW
// Stores a reference counted handle to a window object.


#pragma once
#include "asset.h"


namespace ue {
    struct Window:
        Asset<SDL_Window, void(*)(SDL_Window*), &SDL_DestroyWindow>
    {
        using Asset::Asset;
    };
}