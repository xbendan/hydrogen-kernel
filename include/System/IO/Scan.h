#include <Meta/Cursor.h>
#include <System/Text/EncodingFormat.h>
#include <System/Text/Rune.h>

namespace System::IO {
    using Text::EncodingFormat;
    using Text::Rune;

    template <EncodingFormat E>
    struct Scan
    {
        using Encoding = E;
        using Unit     = typename E::Unit;

        Cursor<Unit> _cursor, _begin;

        constexpr Scan(Unit const* buf, usize len)
            : _cursor{ buf, len }
        {
        }

        bool IsEnd() const { return _cursor.IsEnd(); }

        usize Remain()
        {
            auto curr = _cursor;

            usize result = 0;
            while (curr.Remain()) {
                Rune r;
                if (E::DecodeUnit(r, curr)) {
                    result++;
                }
            }
            return result;
        }

        Rune Peek()
        {
            if (IsEnd()) {
                return '\0';
            }

            Rune r;
            auto curr = _cursor;
            return E::DecodeUnit(r, curr) ? r : Text::Unknown;
        }

        Rune Peek(usize count) {}

        Rune Next()
        {
            if (IsEnd()) {
                return '\0';
            }

            Rune r;
            return E::DecodeUnit(r, _cursor) ? r : Text::Unknown;
        }

        Rune Next(usize count)
        {
            Rune r = '\0';
            for (usize i = 0; i < count; i++) {
                r = Next();
            }
            return r;
        }

        bool SkipOnce(Rune r)
        {
            if (Peek() == r) {
                Next();
                return true;
            }
            return false;
        }

        bool SkipCount(usize count)
        {
            for (usize i = 0; i < count; i++) {
                if (IsEnd()) {
                    return false;
                }
                Next();
            }
            return true;
        }

        bool Skip(Rune r)
        {
            bool result = false;
            if (SkipOnce(r)) {
                result = true;
                while (SkipOnce(r) and !IsEnd())
                    ;
            }
            return result;
        }
    };
}