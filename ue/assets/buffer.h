// BUFFER
// Stores a buffer of raw bytes in memory.


#pragma once
#include "../sdl.h"
#include "asset.h"


namespace ue {
    // This helper function is needed because `SDL_RWclose` is
    // a macro and not a function.
    void cleanup_rw(SDL_RWops* context) {
        SDL_RWclose(context);
    }


    // Holds a pointer to a data source.
    struct Buffer {
        std::unique_ptr<SDL_RWops, void(*)(SDL_RWops*)> ptr;

        Buffer(): ptr(nullptr, &cleanup_rwops) {}
        explicit Buffer(SDL_RWops* ptr_): ptr(ptr_, &cleanup_rwops) {}

        Buffer(const Buffer& obj) = delete;
        Buffer(Buffer&& obj): ptr(std::move(obj.ptr)) {}

        ~Buffer() {}

        Buffer& operator=(Buffer&& obj) noexcept {
            ptr = std::move(obj.ptr);
            return *this;
        }

        bool operator==(const Buffer& other) {
            return raw() == other.raw();
        }

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
    };
}