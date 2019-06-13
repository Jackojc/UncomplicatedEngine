// RENDERER
// Stores a reference counted handle to a renderer object.


#pragma once
#include "asset.h"


namespace ue {
    struct Renderer:
        Asset<SDL_Renderer, void(*)(SDL_Renderer*), &SDL_DestroyRenderer>
    {
        using Asset::Asset;
    };
}