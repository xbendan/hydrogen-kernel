#pragma once

#include <Meta/Tuple.h>
#include <System/Text/Encoding.UTF8.h>
#include <System/Text/EncodingFormat.h>

namespace System::Text {
    template <EncodingFormat E>
    struct StringView
    {
        using Encoding  = E;
        using Unit      = typename E::Unit;
        using InnerType = Unit;

        always_inline constexpr StringView() = default;

        // always_inline constexpr StringView(Unit const* buf)
        //     requires(Same<Unit, char>)
        //     : _buf(buf)
        //     , _len(strnlen(buf, 512))
        // {
        // }

        always_inline constexpr StringView(Unit const* buf, usize len)
            : _buf(buf)
            , _len(len)
        {
        }

        always_inline constexpr StringView(Unit const* begin, Unit const* end)
            : _buf(begin)
            , _len(end - begin)
        {
        }

        always_inline constexpr StringView(Sliceable<Unit> auto& other)
            : _buf(other.buf())
            , _len(other.len())
        {
        }

        always_inline constexpr StringView(StringView const& other)
            : StringView(other._buf, other._len)
        {
        }

        always_inline constexpr StringView(StringView&& other)
            : _buf(Std::exchange(other._buf, nullptr))
            , _len(Std::exchange(other._len, 0))
        {
        }

        always_inline constexpr StringView& operator=(StringView const& other)
        {
            _buf = other._buf;
            _len = other._len;

            return *this;
        }

        always_inline constexpr StringView& operator=(StringView&& other)
        {
            _buf = Std::exchange(other._buf, nullptr);
            _len = Std::exchange(other._len, 0);

            return *this;
        }

        always_inline StringView& operator=(Sliceable<Unit> auto& other)
        {
            _buf = other.buf();
            _len = other.len();

            return *this;
        }

        always_inline StringView& operator=(Unit const* other)
        {
            _buf = other;
            _len = strnlen(other, 512);

            return *this;
        }

        always_inline Unit const& operator[](usize i) const { return _buf[i]; }

        always_inline bool operator==(Sliceable<Unit> auto const& other) const
        {
            if (_len != other.len()) {
                return false;
            }

            for (usize i = 0; i < other.len(); i++) {
                if (_buf[i] != other[i]) {
                    return false;
                }
            }

            return true;
        }

        always_inline bool operator==(Unit const* other) const
        {
            return *this == StringView(other);
        }

        always_inline constexpr explicit operator bool() const
        {
            return _len > 0;
        }

        always_inline Unit* begin() const { return const_cast<Unit*>(_buf); }

        always_inline Unit* end() const
        {
            return const_cast<Unit*>(_buf) + _len;
        }

        Unit const* _buf = nullptr;
        usize       _len = 0;
    };

    using strview = StringView<UTF8>;
}

using System::Text::strview;

always_inline constexpr strview
operator""_sv(char const* str, usize len)
{
    return strview(str, len);
}