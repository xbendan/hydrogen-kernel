#pragma once

#include <System/Text/EncodingFormat.h>
#include <System/Text/Rune.h>

namespace System::Text {
    struct UTF8
    {
        using Unit = char;
        using One  = Multiple<Unit, 4>;

        /**
         * @brief Get the byte length of specific char code
         *
         * This function returns how many bytes are needed for
         * current unit to represent a character. If the unit
         * is not a valid utf-8 unit, 0 will be returned.
         *
         * Compare the first unit with specific masks, if the
         * unit start with 0, it is a single byte character,
         * this is for the compatibility with ASCII. If the
         * unit start with 0xC0 (110 in binary format), it is
         * a two bytes character. The same rule applies to
         * three bytes (0xE0, 1110 in binary) and four bytes
         * character. (0xF0, 11110 in binary)
         *
         * According to the early design of utf-8, the maximum
         * length of a character is 6 bytes, but the range
         * shrunk to 4 bytes by RFC 3629 in 2003. So we only
         * need to handle with 1 to 4 bytes character.
         *
         * @param unit the first byte of the character
         * @return byte length of current character
         */
        static constexpr u8 GetUnitLength(Unit unit)
        {
            if ((unit & 0b1000'0000) == 0x00) {
                return 1;
            } else if ((unit & 0b1110'0000) == 0b1100'0000) {
                return 2;
            } else if ((unit & 0b1111'0000) == 0b1110'0000) {
                return 3;
            } else if ((unit & 0b1111'1000) == 0b1111'0000) {
                return 4;
            } else
                return 0;
        }

        /**
         * @brief Get the rune length of specific rune
         *
         * Rune is an unicode character, it is a 32-bit integer
         * that represents a character. The rune length is the
         * byte length of the rune.
         *
         * The rune under 0x7F is a single byte character, the
         * rune under 0x7FF is a two bytes character, and the
         * same rule applies to three bytes (0xFFFF) and four
         * bytes (more).
         *
         * @param rune
         * @return rune length
         */
        static constexpr u8 GetRuneLength(Rune rune)
        {
            if (rune < 0x80) {
                return 1;
            } else if (rune < 0x800) {
                return 2;
            } else if (rune < 0x10000) {
                return 3;
            } else if (rune < 0x110000) {
                return 4;
            } else
                return 0;
        }

        static bool EncodeUnit(Rune r, EncodeOutput<Unit> auto& unit)
        {
            if (GetUnitLength(r) > unit.Remain()) [[unlikely]]
                return false;

            if (r <= 0b0111'1111) {
                unit.Append(r);
            } else if (r <= 0x7ff) {
                unit.Append(0b1100'0000 | ((r >> 6) & 0b0001'1111));
                unit.Append(0b1000'0000 | (r & 0b0011'1111));
            } else if (r <= 0xffff) {
                unit.Append(0b1110'0000 | ((r >> 12) & 0b0000'1111));
                unit.Append(0b1000'0000 | ((r >> 6) & 0b0011'1111));
                unit.Append(0b1000'0000 | (r & 0b0011'1111));
            } else if (r <= 0x10ffff) {
                unit.Append(0b1111'0000 | ((r >> 18) & 0b0000'0111));
                unit.Append(0b1000'0000 | ((r >> 12) & 0b0011'1111));
                unit.Append(0b1000'0000 | ((r >> 6) & 0b0011'1111));
                unit.Append(0b1000'0000 | (r & 0b0011'1111));
            } else {
                return encodeUnit(Unknown, unit);
            }

            return true;
        }

        static bool DecodeUnit(Rune& r, DecodeInput<Unit> auto& unit)
        {
            if (unit.Remain() == 0) {
                r = Unknown;
                return false;
            }
            Unit u = unit.Next();

            if ((u & 0b1000'0000) == 0x00) {
                r = u;
            } else if ((u & 0b1110'0000) == 0b1100'0000) {
                r = (u & 0b0001'1111) << 6;
                r |= unit.Next() & 0b0011'1111;
            } else if ((u & 0b1111'0000) == 0b1110'0000) {
                r = (u & 0b0000'1111) << 12;
                r |= (unit.Next() & 0b0011'1111) << 6;
                r |= unit.Next() & 0b0011'1111;
            } else if ((u & 0b1111'1000) == 0b1111'0000) {
                r = (u & 0b0000'0111) << 18;
                r |= (unit.Next() & 0b0011'1111) << 12;
                r |= (unit.Next() & 0b0011'1111) << 6;
                r |= unit.Next() & 0b0011'1111;
            } else {
                r = u;
            }

            return true;
        }
    };
    static_assert(EncodingFormat<UTF8>);
}