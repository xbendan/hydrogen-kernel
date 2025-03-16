#pragma once

#include <Concepts.h>
#include <Types.h>

template <typename T>
struct Type
{
    static T* null() { return nullptr; }

    template <typename... Args>
    static T* new$(Args... args)
    {
        return new T(args...);
    }

    static T* new$uninit() { return new T; }

    template <typename U, typename... Args>
        requires ConvertibleTo<U, void*>
    static T* new$at(U addr, Args... args)
    {
        return new (reinterpret_cast<void*>(addr)) T(args...);
    }

    static T* newArray$(usize len) { return new T[len]; }

    static void delete$(T* ptr) { delete ptr; }
};

#ifndef __hosted__

inline void*
operator new(long unsigned int size, void* p) noexcept
{
    return p;
}

inline void*
operator new[](long unsigned int size, void* p) noexcept
{
    return p;
}

#endif