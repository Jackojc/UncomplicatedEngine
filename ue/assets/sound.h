// SOUND
// Stores a reference counted handle to a sound resource.


#pragma once
#include "asset.h"


namespace ue {
    struct Sound:
        Asset<Mix_Chunk, void(*)(Mix_Chunk*), &Mix_FreeChunk>
    {
        using Asset::Asset;
    };
}