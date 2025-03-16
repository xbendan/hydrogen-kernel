#include <Meta/Property.h>
#include <System/Text/Encoding.UTF8.h>
#include <System/Text/EncodingFormat.h>
#include <System/Text/StringView.h>

namespace System::Text {
    template <EncodingFormat E>
    struct String
    {
        using Encoding  = E;
        using Unit      = typename E::Unit;
        using InnerType = Unit;

        constexpr String() = default;

        always_inline constexpr String(Unit const* data, usize len)
            : _data(data)
            , _len(len)
        {
        }

        always_inline constexpr String(Unit const* data)
            : String(data, data ? GetLength(data) : 0)
        {
        }

        always_inline constexpr String(String const& other)
            : String(other._data, other._len)
        {
        }

        always_inline constexpr String(String&& other)
            : _data(other._data)
            , _len(other._len)
        {
            other._data = nullptr;
            other._len  = 0;
        }

        always_inline ~String()
        {
            if (_data) {
                _len = 0;
                delete[] _data;
            }
        }

        always_inline String& operator=(String const& other)
        {
            if (this == &other) {
                return *this;
            }

            if (_data) {
                _len = 0;
                delete[] _data;
            }

            _data = new Unit[other._len];
            _len  = other._len;
            memcpy(_data, other._data, _len);

            return *this;
        }

        always_inline String& operator=(String&& other)
        {
            if (this == &other) {
                return *this;
            }

            if (_data) {
                _len = 0;
                delete[] _data;
            }

            _data = other._data;
            _len  = other._len;

            other._data = nullptr;
            other._len  = 0;

            return *this;
        }

        always_inline String& operator=(Unit const* other)
        {
            if (_data) {
                _len = 0;
                delete[] _data;
            }

            _len  = GetLength(other);
            _data = new Unit[_len];
            memcpy(_data, other, _len);

            return *this;
        }

        always_inline Unit& operator[](usize i)
        {
            if (i >= _len) [[unlikely]] {
                Std::terminate("String::operator[]: index out of bounds");
            }

            return const_cast<char&>(_data[i]);
        }

        always_inline Unit const& operator[](usize i) const
        {
            if (i >= _len) [[unlikely]] {
                Std::terminate("String::operator[]: index out of bounds");
            }

            return _data[i];
        }

        static constexpr usize GetLength(Unit const* data, usize max = 512)
        {
            Cursor<Unit> cursor{ data };
            usize        len = 0;
            while (cursor.Remain() and len < max) {
                Rune r;
                if (E::DecodeUnit(r, cursor))
                    len++;
            }
            return len;
        }

        always_inline constexpr explicit operator bool() const
        {
            return _len > 0;
        }

        always_inline constexpr explicit operator strview() const
        {
            return strview{ _data, _len };
        }

        Unit* begin() const { return const_cast<Unit*>(_data); }

        Unit* end() const { return const_cast<Unit*>(_data) + _len; }

        Unit const* _data = nullptr;
        usize       _len{ 0 };
    };

    using string = String<UTF8>;
}

using System::Text::string;