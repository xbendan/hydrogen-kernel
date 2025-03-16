#pragma once

#include <TypeTraits.h>

namespace Std {
    template <typename T>
    constexpr T&& forward(RemoveRef<T>& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr T&& forward(RemoveRef<T>&& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr typename Std::RemoveRef<T>&& move(T&& t) noexcept
    {
        return static_cast<typename Std::RemoveRef<T>&&>(t);
    }

    template <typename T1, typename T2 = T1>
    constexpr inline T1 exchange(T1& _old, T2&& _new)
    {
        T1 old = move(_old);
        _old   = forward<T2>(_new);
        return old;
    }
}