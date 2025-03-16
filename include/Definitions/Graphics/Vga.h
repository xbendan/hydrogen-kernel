#pragma once

#include <Types.h>

namespace VGA {
    static constexpr u8 sc_TextWidth  = 80;
    static constexpr u8 sc_TextHeight = 25;
    static const u16*   sc_TextMemory = (u16*)(0xB8000);

    enum class TextColor : u8
    {
        Black        = 0,
        Blue         = 1,
        Green        = 2,
        Cyan         = 3,
        Red          = 4,
        Magenta      = 5,
        Brown        = 6,
        LightGray    = 7,
        DarkGray     = 8,
        LightBlue    = 9,
        LightGreen   = 10,
        LightCyan    = 11,
        LightRed     = 12,
        LightMagenta = 13,
        Yellow       = 14,
        White        = 15
    };
}