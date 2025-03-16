#pragma once

#include <System/Text/EncodingFormat.h>
#include <System/Text/Rune.h>

namespace System::Text {
    struct UTF16
    {
        using Unit = u16;
        using One  = Multiple<Unit, 2>;

        static constexpr u8 GetUnitLength(Unit unit)
        {
            if ((unit & 0xFC00) == 0xD800) {
                return 2;
            } else
                return 1;
        }

        static constexpr u8 GetRuneLength(Rune rune)
        {
            if (rune < 0x10000) {
                return 1;
            } else if (rune < 0x110000) {
                return 2;
            } else
                return 0;
        }

        static bool EncodeUnit(Rune r, EncodeOutput<Unit> auto& unit)
        {
            if (r <= 0xffff) {
                unit.Append(r);
                return true;
            } else if (r <= 0x10ffff) {
                unit.Append(0xD800 | ((r - 0x10000) >> 10));
                unit.Append(0xDC00 | ((r - 0x10000) & 0x3FF));
                return true;
            }
            return encodeUnit(Unknown, unit);
        }

        static bool DecodeUnit(Rune& r, DecodeInput<Unit> auto& unit)
        {
            Unit u = unit.Next();

            if (GetUnitLength(u) > unit.Remain()) {
                r = Unknown;
                return false;
            }

            if (u >= 0xd800 && u <= 0xdbff) {
                if (unit.Remain() < 2) {
                    return false;
                }
                Unit u2 = unit.Next();
                if (u2 < 0xdc00 || u2 > 0xdfff) {
                    return false;
                }

                r = 0x10000 + ((u & 0x3ff) << 10) + (u2 & 0x3ff);
            } else {
                r = u;
            }

            return true;
        }
    };
    static_assert(EncodingFormat<UTF16>);
}