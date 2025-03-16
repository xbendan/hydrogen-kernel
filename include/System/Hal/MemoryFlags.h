#pragma once

#if defined(__x86_64__)

#define HAL_NAMESPACE x64
#include <System/Arch/x64/Paging.h>

#elif defined(HAL_LOONGARCH64)

#endif

namespace System::Hal {
    enum class PmmFlags : u8
    {
        READY           = 1 << 0,
        KERNEL_RESERVED = 1 << 1,
        PAGE_MIGRATED   = 1 << 2,
        PAGE_SWAPPED    = 1 << 3,
        SHARING         = 1 << 4,
        WRITE_PROTECTED = 1 << 5,
    };
    MakeFlags$(PmmFlags);

    using VmmFlags = HAL_NAMESPACE::VmmFlags;
    using VmmEntry = HAL_NAMESPACE::VmmEntry;

    enum class PmmRegionType : u8
    {
    };
}