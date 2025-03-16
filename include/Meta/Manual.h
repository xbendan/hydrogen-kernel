#pragma once

#include <Types.h>
#include <Utility.h>

namespace Meta {
    template <typename T>
    struct Manual
    {
        using InnerType = T;

        alignas(alignof(T)) byte _data[sizeof(T)];

        ~Manual() { Get().~T(); }

        template <typename... Args>
        always_inline T* operator()(Args&&... args)
        {
            return new (&Get()) T{ Std::forward<Args>(args)... };
        }

        always_inline T& Get() { return *(T*)_data; }

        always_inline T const& Get() const { return *(T const*)_data; }

        always_inline T* operator->() { return &Get(); }

        always_inline T const* operator->() const { return &Get(); }

        always_inline T& operator*() { return Get(); }

        always_inline T const& operator*() const { return Get(); }

        always_inline operator T&() { return Get(); }

        always_inline operator T const&() const { return Get(); }

        always_inline T Take()
        {
            T result = Get();
            ~Manual();
            return result;
        }
    };
}

using Meta::Manual;