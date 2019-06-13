#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <stdexcept>












/*
    SDL includes.
*/

extern "C" {
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_mixer.h>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_net.h>
}
























/*
    CONSTANTS
*/

namespace ue {
    //constexpr auto WINDOW_UNDEFINED = SDL_WINDOWPOS_UNDEFINED;

}





















/*
    TYPE ALIASES
*/

namespace ue {

}




























/*
    EXCEPTION TYPES
*/

namespace ue {
    std::string sdl_error() { return std::string{SDL_GetError()}; }
    std::string img_error() { return std::string{IMG_GetError()}; }
    std::string mix_error() { return std::string{Mix_GetError()}; }
    std::string ttf_error() { return std::string{TTF_GetError()}; }
    std::string net_error() { return std::string{SDLNet_GetError()}; }
}


#define UE_NEW_RUNTIME_EXCEPTION_TYPE(name, default_msg) \
    class name: public std::runtime_error { \
        public: \
            name(const std::string& msg): std::runtime_error(msg) {} \
            name(): std::runtime_error(default_msg) {} \
    };


#define UE_NEW_LOGIC_EXCEPTION_TYPE(name, default_msg) \
    class name: public std::logic_error { \
        public: \
            name(const std::string& msg): std::logic_error(msg) {} \
            name(): std::logic_error(default_msg) {} \
    };


namespace ue {
    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorLoadFile,
        "Could not load the file!"
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorLoadSurfaceFromFile,
        "Could not load the surface from the file! " + img_error()
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorLoadTextureFromFile,
        "Could not load the texture from the file! " + sdl_error()
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorLoadSoundFromFile,
        "Could not load the sound from the file! " + mix_error()
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorLoadMusicFromFile,
        "Could not load the music from the file! " + mix_error()
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorLoadFontFromFile,
        "Could not load the font from the file! " + ttf_error()
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorSizeOfFile,
        "Could not determine the size of the file! " + sdl_error()
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorCreateWindow,
        "Could not create a window! " + sdl_error()
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorCreateRenderer,
        "Could not create a renderer! " + sdl_error()
    );


    UE_NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorCreateTarget,
        "Could not create a rendering target! " + sdl_error()
    );
}


#undef UE_NEW_LOGIC_EXCEPTION_TYPE
#undef UE_NEW_RUNTIME_EXCEPTION_TYPE






















/*
    file
    Operates as a kind of file handle.
*/

namespace ue {
    // This helper function is needed because `SDL_RWclose` is
    // a macro and not a function.
    void cleanup_rwops(SDL_RWops* context) {
        SDL_RWclose(context);
    }


    // Holds a pointer to a data source.
    struct File {
        std::string filename;
        std::unique_ptr<SDL_RWops, void(*)(SDL_RWops*)> ptr;


        File(const std::string& filename_): ptr(nullptr, &cleanup_rwops) {
            ptr.reset(SDL_RWFromFile(filename_.c_str(), "rb"));

            if (not ptr) throw ue::ErrorLoadFile();
        }


        File(const ue::File& obj) = delete;
        File(ue::File&& obj): ptr(std::move(obj.ptr)) {}


        ~File() {}


        // Operators.
        ue::File& operator=(ue::File&& obj) noexcept {
            ptr = std::move(obj.ptr);
            return *this;
        }

        bool operator==(const ue::File& other) {
            return filename == other.filename;
        }


        // Implicit type conversions.
        operator bool() const {
            return bool(ptr);
        }

        operator SDL_RWops*() const {
            return ptr.get();
        }

        operator SDL_RWops() const {
            return *ptr.get();
        }

        SDL_RWops* raw() const {
            return ptr.get();
        }


        // IO operations.
        Sint64 size() {
            auto size = SDL_RWsize(raw());

            if (size < 0) throw ue::ErrorSizeOfFile();

            return size;
        }

        void* read() {
            auto res_size = size();
            char* res = static_cast<char*>(::operator new(res_size));


            Sint64 nb_read_total = 0,
                   nb_read = 1;


            char* buff = res;

            while (nb_read_total < res_size && nb_read != 0) {
                nb_read = SDL_RWread(
                    raw(), buff, 1, (res_size - nb_read_total)
                );

                nb_read_total += nb_read;
                buff += nb_read;
            }


            if (nb_read_total != res_size) {
                ::operator delete(res);
                SDL_RWseek(raw(), 0, RW_SEEK_SET);
                throw ue::ErrorLoadFile();
            }


            SDL_RWseek(raw(), 0, RW_SEEK_SET);
            return static_cast<void*>(res);
        }
    };
}






























/*
    ASSET
*/

namespace ue {
    // Convenience alias because function pointers are ugly.
    template <typename T>
    using SDL_FreeFunction = void(*)(T);


    // Implements a container for SDL resources.
    template <typename T, typename C, C free>
    struct Asset {
        std::shared_ptr<T> ptr;


        Asset(): ptr(nullptr, free) {}
        Asset(T* ptr_): ptr(ptr_, free) {}
        Asset(const ue::Asset<T, C, free>& asset): ptr(asset.ptr) {}

        /*template <typename L, typename... Ts>
        Asset(L loader, const ue::File& file, Ts&&... args):
            ptr(nullptr, free)
        {
            ptr.reset(loader(file, std::forward<Ts>(args)...), free);
        }


        template <typename R, typename... Ts>
        Asset(R(*loader)(Ts...), Ts&&... args):
            ptr(nullptr, free)
        {
            ptr.reset(loader(std::forward<Ts>(args)...), free);
        }*/


        virtual ~Asset() {}


        operator bool() const {
            return bool(ptr);
        }

        operator T() const {
            return *ptr.get();
        }

        operator T*() const {
            return ptr.get();
        }

        std::weak_ptr<T> weak() const {
            return ptr;
        }

        T* raw() const {
            return ptr.get();
        }
    };
}


























/*
    CORE
*/

namespace ue {
    // Forward declarations.
    struct Window;
    struct Target;
    struct Renderer;





    // A wrapper for an SDL_Window.
    struct Window:
        Asset<SDL_Window, void(*)(SDL_Window*), &SDL_DestroyWindow>
    {
        // Members
        // ...
        int w, h;
        Uint32 flags;


        // Constructors
        using Asset::Asset;

        Window(const std::string& caption, int w_, int h_, Uint32 flags_ = 0);
    };


    // A wrapper for an SDL_Renderer.
    struct Target:
        Asset<SDL_Texture, void(*)(SDL_Texture*), &SDL_DestroyTexture>
    {
        // Members
        // ...
        int w, h;


        // Constructors
        using Asset::Asset;

        Target(const ue::Renderer& renderer, int w_, int h_);
    };


    // A wrapper for an SDL_Renderer.
    struct Renderer:
        Asset<SDL_Renderer, void(*)(SDL_Renderer*), &SDL_DestroyRenderer>
    {
        // Members
        // ...
        Uint32 flags;


        // Constructors
        using Asset::Asset;

        Renderer(const ue::Window& window, Uint32 flags_ = 0);


        // Functions.
        // void draw(sprite);;

        // void resize();
        // void set_size();
        // etc...
    };


    // A class to manage a window and renderer as a single entity.
    struct Canvas:
        ue::Window,
        ue::Renderer
    {
        // Members
        // ...


        // Constructors
        //using Renderer::Renderer;
        //using Window::Window;

        Canvas(
            const std::string& caption,
            int w_, int h_,
            Uint32 window_flags_ = 0, Uint32 render_flags_ = 0
        ):
            Window::Window(caption, w_, h_, window_flags_)
            //Renderer::Renderer(window, render_flags_)
        {

        }


        // Functions
    };
}





















/*
    Create utils.
*/

namespace ue {
    // Create an SDL_Window.
    SDL_Window* create_window(
        const std::string& caption,
        int w, int h,
        Uint32 flags = 0
    ) {
        auto ptr = SDL_CreateWindow(
            caption.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            w, h,
            flags
        );

        if (not ptr) throw ue::ErrorCreateWindow();
        return ptr;
    }


    // Create an SDL_Renderer.
    SDL_Renderer* create_renderer(
        const ue::Window& window,
        Uint32 flags = 0
    ) {
        auto ptr = SDL_CreateRenderer(
            window, -1,
            flags
        );

        if (not ptr) throw ue::ErrorCreateRenderer();
        return ptr;
    }


    // Create an SDL_Texture for rendering to.
    SDL_Texture* create_target(
        const ue::Renderer& renderer,
        int w, int h
    ) {
        auto ptr = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            w, h
        );

        if (not ptr) throw ue::ErrorCreateTarget();
        return ptr;
    }
}




















/*
    CORE DEFINITIONS
*/

namespace ue {
    Window::Window(const std::string& caption, int w_, int h_, Uint32 flags_):
        Asset::Asset(ue::create_window(caption, w_, h_, flags_)),
        w(w_), h(h_),
        flags(flags_)
    {

    }


    Target::Target(const ue::Renderer& renderer, int w_, int h_):
        Asset::Asset(ue::create_target(renderer, w_, h_)),
        w(w_), h(h_)
    {

    }


    Renderer::Renderer(const ue::Window& window, Uint32 flags_):
        Asset::Asset(
            ue::create_renderer(
                window,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | flags_
            )
        ),
        flags(flags_)
    {

    }
}




















/*
    Loader utils.
*/

namespace ue {
    // Loader for surfaces.
    SDL_Surface* file_to_surface(const ue::File& file) {
        auto ptr = IMG_Load_RW(file, 0);

        SDL_RWseek(file.raw(), 0, RW_SEEK_SET);

        if (not ptr) throw ue::ErrorLoadSurfaceFromFile();
        return ptr;
    }


    // Loader for textures.
    SDL_Texture* file_to_texture(
        const ue::File& file,
        const ue::Renderer& renderer
    ) {
        auto ptr = SDL_CreateTextureFromSurface(
            renderer, ue::file_to_surface(file)
        );

        if (not ptr) throw ue::ErrorLoadTextureFromFile();
        return ptr;
    }


    // Loader for sound effects.
    Mix_Chunk* file_to_sound(const ue::File& file) {
        auto ptr = Mix_LoadWAV_RW(file, 0);

        SDL_RWseek(file.raw(), 0, RW_SEEK_SET);

        if (not ptr) throw ue::ErrorLoadSoundFromFile();
        return ptr;
    }


    // Loader for music.
    Mix_Music* file_to_music(const ue::File& file) {
        auto ptr = Mix_LoadMUS(file.filename.c_str());

        SDL_RWseek(file.raw(), 0, RW_SEEK_SET);

        if (not ptr) throw ue::ErrorLoadMusicFromFile();
        return ptr;
    }


    // Loader for fonts.
    TTF_Font* file_to_font(const ue::File& file, int size) {
        auto ptr = TTF_OpenFontRW(file, 0, size);

        SDL_RWseek(file.raw(), 0, RW_SEEK_SET);

        if (not ptr) throw ue::ErrorLoadFontFromFile();
        return ptr;
    }
}








































/*
    ASSETS
*/

namespace ue {
    // A wrapper for an SDL_Texture.
    struct Sprite:
        Asset<SDL_Texture, void(*)(SDL_Texture*), &SDL_DestroyTexture>
    {
        // Members
        int x, y, w, h;

        // Constructors
        using Asset::Asset;

        Sprite(const ue::File& file, const ue::Renderer& renderer):
            Asset::Asset(ue::file_to_texture(file, renderer)) {}
    };


    // A wrapper for a Mix_Chunk.
    struct Cue:
        Asset<Mix_Chunk, void(*)(Mix_Chunk*), &Mix_FreeChunk>
    {
        // Members
        // ...

        // Constructors
        using Asset::Asset;

        Cue(const ue::File& file): Asset::Asset(ue::file_to_sound(file)) {}
    };


    // A wrapper for a Mix_Music.
    struct Track:
        Asset<Mix_Music, void(*)(Mix_Music*), &Mix_FreeMusic>
    {
        // Members
        // ...

        // Constructors
        using Asset::Asset;

        Track(const ue::File& file): Asset::Asset(ue::file_to_music(file)) {}
    };


    // A wrapper for a TTF_Font.
    struct Face:
        Asset<TTF_Font, void(*)(TTF_Font*), &TTF_CloseFont>
    {
        // Members
        // ...

        // Constructors
        using Asset::Asset;

        Face(const ue::File& file, int size):
            Asset::Asset(ue::file_to_font(file, size)) {}
    };
}





























/*
    CONTEXT
*/

namespace ue {
    template <typename T>
    struct Context:
        ue::Renderer,
        ue::Window
    {

    };
}















