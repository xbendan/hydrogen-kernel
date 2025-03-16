#pragma once

#include <Types.h>

namespace System {
    union Uuid
    {
        struct
        {
            u32 _low;
            u16 _mid;
            u16 _hi;
            u16 _hi2;
            u64 _hi3 : 48;
        } __attribute__((packed));
        u8  _bytes[16];
        u64 _quad[2];

        constexpr inline bool operator==(Uuid const& other) const
        {
            return _quad[0] == other._quad[0] && _quad[1] == other._quad[1];
        }

        constexpr inline bool operator!=(Uuid const& other) const
        {
            return _quad[0] != other._quad[0] || _quad[1] != other._quad[1];
        }

        static const Uuid sc_null;
        static const Uuid sc_nil;

        static constexpr Uuid fromString(char const* str);
        static constexpr Uuid random();
    };

    Uuid const Uuid::sc_null = { 0, 0, 0, 0, 0 };
    Uuid const Uuid::sc_nil  = { 0, 0, 0, 0, 0 };

    Uuid constexpr Uuid::fromString(char const* str)
    {
        {
            Uuid uuid = { 0, 0, 0, 0, 0 };
            u8*  p    = uuid._bytes;
            for (int i = 0; i < 36; i++) {
                if (i == 8 || i == 13 || i == 18 || i == 23) {
                    if (str[i] != '-')
                        return sc_nil;
                } else {
                    if (str[i] >= '0' && str[i] <= '9')
                        *p = (*p << 4) | (str[i] - '0');
                    else if (str[i] >= 'a' && str[i] <= 'f')
                        *p = (*p << 4) | (str[i] - 'a' + 10);
                    else if (str[i] >= 'A' && str[i] <= 'F')
                        *p = (*p << 4) | (str[i] - 'A' + 10);
                    else
                        return sc_nil;
                    if ((i & 1) == 1)
                        p++;
                }
            }
            return uuid;
        }
    }
}