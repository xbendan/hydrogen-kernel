#pragma once

#include <Types.h>

namespace x64 {
    struct FxState
    {
        u16 _fcw;
        u16 _fsw;
        u8  _ftag;
        u8  __reserved__1;
        u16 _fop;
        u64 _rip;
        u64 _rdp;
        u32 _mxcsr;
        u32 _mxcsr_mask;
        u8  _st[0x08][0x10];
        u8  _xmm[0x10][0x10];
    };
}