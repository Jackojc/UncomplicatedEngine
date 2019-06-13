#pragma once
#include <memory>
#include <functional>
#include <utility>


namespace ue {
    // Convenience alias because function pointers are ugly.
    template <typename T>
    using SDL_FreeFunction = void(*)(T);


    // Implements a container for SDL assets.
    template <typename T, typename C, C free>
    struct Asset {
        std::shared_ptr<T> ptr;


        Asset(): ptr(nullptr, free) {}
        Asset(T* ptr_): ptr(ptr_, free) {}
        Asset(const Asset<T, C, free>& asset): ptr(asset.ptr) {}

        template <typename L>
        Asset(L loader, const ue::Buffer& buffer): ptr(nullptr, free) {
            ptr.reset(loader(buffer));
        }


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