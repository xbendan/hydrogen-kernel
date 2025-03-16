#pragma once

#include <System/Buffer.h>
#include <Types.h>

namespace System::Text {
    using Rune = u32;

    static constexpr Rune Unknown = U'�';

    template <typename U, usize TLen>
    struct Multiple
    {
        using InnerType = U;
        using Unit      = U;

        Buffer<Unit, TLen> _buf{};

        always_inline void Append(Unit u) { _buf += u; }

        always_inline constexpr Unit& operator[](usize i) { return _buf[i]; }

        always_inline constexpr Unit const& operator[](usize i) const
        {
            return _buf[i];
        }

        always_inline constexpr Unit* Inner() { return _buf.begin(); }

        always_inline constexpr Unit const* Inner() const
        {
            return _buf.begin();
        }

        always_inline constexpr usize Length() const { return _buf.len(); }

        always_inline constexpr usize Remain() const { return TLen - Length(); }
    };

    template <typename U>
    struct Single
    {
        using InnerType = U;
        using Unit      = U;

        Unit _buf;

        always_inline Single() = default;

        always_inline Single(Unit u)
            : _buf(u)
        {
        }

        always_inline void Append(Unit u) { _buf = u; }

        always_inline operator Unit() { return _buf; }

        always_inline constexpr Unit& operator[](usize) { return _buf; }

        always_inline constexpr Unit const& operator[](usize) const
        {
            return _buf;
        }

        always_inline constexpr Unit* Inner() { return &_buf; }

        always_inline constexpr Unit const* Inner() const { return &_buf; }

        always_inline constexpr usize Length() const { return 1; }
    };
}