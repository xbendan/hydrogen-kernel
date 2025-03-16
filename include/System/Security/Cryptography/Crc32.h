#pragma once

#include <Types.h>

namespace System::Crypto {
    u32 Crc32(u8 const* data, usize len, u32 seed);
}