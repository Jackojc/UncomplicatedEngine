// TEXTURE
// Stores a reference counted handle to a texture resource.


#pragma once
#include "asset.h"


namespace ue {
    struct Texture:
        Asset<SDL_Texture, void(*)(SDL_Texture*), &SDL_DestroyTexture>
    {
        using Asset::Asset;
    };


    ue::Texture make_texture() {

    }


    ue::Texture make_texture_from_buffer() {

    }


    ue::Texture make_texture_from_file() {

    }
}