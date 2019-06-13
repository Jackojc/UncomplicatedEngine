// MUSIC
// Stores a reference counted handle to a music resource.


#pragma once
#include "asset.h"


namespace ue {
    struct Music:
        Asset<Mix_Music, void(*)(Mix_Music*), &Mix_FreeMusic>
    {
        using Asset::Asset;
    };
}