#pragma once

#include <Meta/Flags.h>

namespace x64 {
    using VmmEntry = u64;

    enum class VmmFlags : u64
    {
        PRESENT         = (1 << 0),
        WRITABLE        = (1 << 1),
        USER            = (1 << 2),
        WRITE_THROUGH   = (1 << 3),
        CACHE_DISABLED  = (1 << 4),
        ACCESSED        = (1 << 5),
        DIRTY           = (1 << 6),
        PAGE_SIZE       = (1 << 7),
        GLOBAL          = (1 << 7),
        DISABLE_EXECUTE = (1 << 11),

        // Combinations
        USER_RW = PRESENT | WRITABLE | USER,
        USER_RO = PRESENT | USER,
        KERNEL  = PRESENT | WRITABLE
    };
    MakeFlags$(VmmFlags);

    template <bool isKernel = false>
    struct PagingData
    {};
}