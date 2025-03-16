#pragma once

#define PAGE_SIZE_4K 0x1000
#define PAGE_SIZE_2M 0x200000
#define PAGE_SIZE_1G 0x40000000

#if defined(__x86_64__)

#define HAL_NAMESPACE x64
#define PAGE_FRAME_SIZE PAGE_SIZE_4K
#include <System/Arch/x64/Paging.h>

#elif defined(HAL_LOONGARCH64)

#endif

namespace System::Hal {
    template <bool isKernel = false>
    using PagingData = HAL_NAMESPACE::PagingData<isKernel>;
}