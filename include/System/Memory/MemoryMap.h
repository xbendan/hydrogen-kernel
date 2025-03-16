#pragma once

#include <Meta/Array.h>
#include <Meta/Tuple.h>
#include <System/Memory/MemoryRegion.h>
#include <Types.h>

namespace System::MemoryService {
    enum class MemoryMapType : u8
    {
        Available,
        Reserved,
        ACPIReclaimable,
        ACPINVS,
        BadMemory,

        // Extended
        Bootloader,
        FrameBuffer,
        KernelOrModule,
    };

    using MemoryMapEntry = Tuple<MemoryRegion, MemoryMapType>;

    using MemoryMap = Array<MemoryMapEntry, 256>;
}