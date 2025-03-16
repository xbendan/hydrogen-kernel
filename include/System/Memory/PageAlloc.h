#include <Meta/Array.h>
#include <Meta/Property.h>
#include <Meta/Result.h>
#include <System/Concurrent/Atomic.h>
#include <System/Concurrent/Spinlock.Scoped.h>
#include <System/Memory/MemoryRegion.h>
#include <System/Memory/PageFrame.h>
#include <System/Memory/PageQueue.h>

namespace System::MemoryService {
    using Concurrent::Atomic;
    using Concurrent::Spinlock;

    static constexpr uptr  sc_pageBiosReserved = 0x100000;
    static constexpr usize sc_MaxQueueLevel    = 10;
    static constexpr usize sc_MaxChainSize     = 1 << sc_MaxQueueLevel;
    static constexpr usize sc_PageLengthLimit  = 0x400;

    PageFrame* AllocatePhysFrame4K(usize amount);
    uptr       AllocatePhysMemory4K(usize amount);
    void       FreePhysFrame4K(PageFrame* page);
    void       FreePhysMemory4K(uptr address);
    void       FreePhysMemory4K(uptr address, usize size);

    u64        MarkRegionAsUsed(MemoryRegion region);
    u64        MarkRegionAsFree(MemoryRegion region);
    PageQueue* FindQueueByLevel(
        Array<PageQueue, sc_MaxQueueLevel + 1> const& zone,
        u8&                                           level);
}