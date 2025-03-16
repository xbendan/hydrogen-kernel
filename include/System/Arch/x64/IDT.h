#pragma once

#include <Meta/Flags.h>
#include <Types.h>

namespace x64 {
    struct InterruptDescTbl
    {
        static constexpr usize Count = 256;

        enum class Flags : u8
        {
            INTERRUPTS = 0b1000'1110,
            TRAP       = 0b1110'1111,
            USER       = 0b0110'0000,
        };

        struct Pack
        {
            u16 _size;
            u64 _offset;
        } __attribute__((packed));

        struct Entry
        {
            u16 _baseLow;
            u16 _selector;
            u8  _ist;
            u8  _flags;
            u16 _baseMedium;
            u32 _baseHigh;
            u32 __reserved__0;

            constexpr Entry() {}

            constexpr Entry(u64 base, u16 selector, u8 flags, u8 ist = 0)
                : _baseLow(base & 0xffff)
                , _selector(selector)
                , _ist(ist & 0x7)
                , _flags(flags)
                , _baseMedium((base >> 16) & 0xffff)
                , _baseHigh((base >> 32) & 0xffffffff)
                , __reserved__0(0)
            {
            }
        } __attribute__((packed)) _ents[256];
    } __attribute__((packed));

    MakeFlags$(InterruptDescTbl::Flags);
}