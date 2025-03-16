#pragma once

#include <System/Text/EncodingFormat.h>
#include <System/Text/Rune.h>

namespace System::Text {
    struct ASCII
    {
        using Unit = char;
        using Code = Single<Unit>;

        static constexpr u8 GetUnitLength(Unit) { return 1; }

        static constexpr u8 GetRuneLength(Rune) { return 1; }

        static bool EncodeUnit(Rune r, EncodeOutput<Unit> auto& unit)
        {
            if (r > 0x7f) {
                unit.Append('?');
                return false;
            }
            unit.Append(r);

            return true;
        }

        static bool DecodeUnit(Rune& r, DecodeInput<Unit> auto& unit)
        {
            if (unit.Remain() == 0) {
                r = '?';
                return false;
            }
            r = unit.Next();
            return true;
        }
    };
    static_assert(EncodingFormat<ASCII>);
}