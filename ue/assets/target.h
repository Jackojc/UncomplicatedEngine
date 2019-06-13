// RENDER TARGET
// Stores a reference counted handle to a texture resource to be used as a rendering target.


#pragma once
#include "asset.h"


namespace ue {
    struct Target:
        Asset<SDL_Texture, void(*)(SDL_Texture*), &SDL_DestroyTexture>
    {
        using Asset::Asset;
    };
}