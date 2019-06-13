// FONTFILE
// Stores a reference counted handle to a font file.


#pragma once
#include "asset.h"


namespace ue {
    // This helper function is needed because `SDL_RWclose` is
    // a macro and not a function.
    void cleanup_rw(SDL_RWops* context) {
        SDL_RWclose(context);
    }


    struct FontFile:
        Asset<SDL_RWops, void(*)(SDL_RWops*), &cleanup_rw>
    {
        using Asset::Asset;
    };


    // Load a font file from in memory.
    ue::FontFile make_font(void* mem, int size) {
        auto ptr = ue::FontFile(
            SDL_RWFromMem(mem, size)
        );

        if (not ptr) throw ue::ErrorCreateFontFile();

        return ptr;
    }


    // Load a font file from disk.
    ue::FontFile make_font_from_file(const std::string& filename) {
        auto ptr = ue::FontFile(
            SDL_RWFromFile(filename.c_str(), "rb")
        );

        if (not ptr) throw ue::ErrorCreateFontFileFromFile();

        return ptr;
    }
}