// FONTFACE
// Stores a font at a specific size.


#pragma once
#include "asset.h"


namespace ue {
    struct FontFace:
        Asset<TTF_Font, void(*)(TTF_Font*), &TTF_CloseFont>
    {
        using Asset::Asset;
    };
}