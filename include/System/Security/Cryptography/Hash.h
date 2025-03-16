#pragma once

#include <Types.h>

namespace System::Crypto {
    i32 GetPrime(i32 value);

    u32 MurmurHash3x86(const void* key, usize len, u32 seed = 0);

    u32 FastHash(i8 const* data, usize len);
}