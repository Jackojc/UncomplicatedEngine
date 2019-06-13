// SURFACE
// Stores a surface in main memory.


#pragma once
#include "asset.h"


namespace ue {
    struct Surface:
        Asset<SDL_Surface, void(*)(SDL_Surface*), &SDL_FreeSurface>
    {
        using Asset::Asset;
    };


    // Create a new surface in memory.
    ue::Surface make_surface(int w, int h) {
        auto ptr = ue::Surface(
            SDL_CreateRGBSurfaceWithFormat(
                0, w, h, 32, SDL_PIXELFORMAT_RGBA8888
            )
        );

        if (not ptr) throw ue::ErrorCreateSurface();

        return ptr;
    }


    // Create a surface from a SDL_RWops buffer.
    ue::Surface make_surface_from_buffer() {

    }


    // Load an image from disk to memory as a surface.
    ue::Surface make_surface_from_file(const std::string& filename) {
        auto ptr = ue::Surface(
            IMG_Load(filename.c_str())
        );

        if (not ptr) throw ue::ErrorCreateSurface();

        return ptr;
    }
}