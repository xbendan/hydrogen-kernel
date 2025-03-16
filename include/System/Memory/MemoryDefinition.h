#include <Meta/Tuple.h>
#include <System/Memory/MemoryRegion.h>
#include <Types.h>

namespace System::MemoryService {
    enum class MemoryRegionDefinitions : u8
    {
        BIOS,
        DMA,
        DMA32,
        NORMAL,
        HIGHMEM,
        RESERVED,
    };

    static constexpr MemoryRegion sc_zoneRegions[] = {
        [0] = { 0x00000000, 0x0009FFFF }, // BIOS
        [1] = { 0x00100000, 0x0FFFFFFF }, // DMA
        [2] = { 0x10000000, 0x3FFFFFFF }, // DMA32
        [3] = { 0x40000000, 0x7FFFFFFF }, // NORMAL
        [4] = { 0x80000000, 0xFFFFFFFF }, // HIGHMEM
        [5] = { 0x00000000, 0xFFFFFFFF }, // RESERVED
    };
}