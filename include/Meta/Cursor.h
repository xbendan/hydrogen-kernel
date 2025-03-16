#pragma once

#include <Meta/Slice.h>
#include <Terminate.h>
#include <Types.h>

namespace Meta {
    template <typename T>
    struct Cursor
    {
        using InnerType = T;

        T* _begin = nullptr;
        T* _end   = nullptr;

        constexpr Cursor() = default;

        always_inline constexpr Cursor(T const* ptr)
            : Cursor(ptr, ptr ? 1 : 0)
        {
        }

        always_inline constexpr Cursor(T const* ptr, usize len)
            : _begin(ptr)
            , _end(ptr + len)
        {
            if (_begin == nullptr and _begin != _end) [[unlikely]]
                Std::terminate("null pointer with non-zero length");
        }

        always_inline constexpr Cursor(T const* begin, T const* end)
            : _begin{ const_cast<T*>(begin) }
            , _end{ const_cast<T*>(end) }
        {
        }

        always_inline constexpr Cursor(Sliceable<T> auto const& slice)
            : Cursor{ slice.buf(), slice.buf() + slice.len() }
        {
        }

        always_inline constexpr T& operator[](usize i)
        {
            if (i >= len()) [[unlikely]]
                Std::terminate("index out of bounds");
            return _begin[i];
        }

        always_inline constexpr T const& operator[](usize i) const
        {
            if (i >= len()) [[unlikely]]
                Std::terminate("index out of bounds");
            return _begin[i];
        }

        always_inline constexpr operator T*() { return _begin; }

        always_inline constexpr operator T const*() const { return _begin; }

        always_inline bool IsEnd() const { return _begin == _end; }

        always_inline constexpr usize Remain() const { return _end - _begin; }

        always_inline constexpr T& Peek(usize i = 0)
        {
            if (i >= len()) [[unlikely]]
                Std::terminate("index out of bounds");
            return _begin[i];
        }

        always_inline constexpr T const& Peek(usize i = 0) const
        {
            if (i >= len()) [[unlikely]]
                Std::terminate("index out of bounds");
            return _begin[i];
        }

        always_inline constexpr T& operator*() { return Peek(); }

        always_inline constexpr T* operator->() { return &Peek(); }

        always_inline constexpr T const& operator*() const { return Peek(); }

        always_inline constexpr T const* operator->() const { return &Peek(); }

        always_inline constexpr T& Next()
        {
            if (IsEnd()) [[unlikely]]
                Std::terminate("next() called on ended cursor");

            T& r = *_begin;
            _begin++;
            return r;
        }

        always_inline constexpr Slice<T> Next(usize n)
        {
            if (n > Remain()) [[unlikely]]
                Std::terminate("next() called on ended cursor");

            auto slice = Slice<T>{ _begin, n };
            _begin += n;
            return slice;
        }

        always_inline constexpr bool Append(T c)
        {
            if (_begin == _end) {
                return true;
            }

            *_begin++ = c;
            return false;
        }

        always_inline constexpr T* buf() { return _begin; }

        always_inline constexpr T const* buf() const { return _begin; }

        always_inline constexpr usize len() const { return _end - _begin; }

        always_inline constexpr Bytes GetAsBytes() const
        {
            return Bytes{ _begin, _end };
        }
    };
}

using Meta::Cursor;