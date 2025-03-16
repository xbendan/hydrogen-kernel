#include <Assertion.h>
#include <Meta/Manual.h>
#include <System/Concurrent/Spinlock.Scoped.h>
#include <System/Memory/MemoryZone.h>
#include <System/Memory/PageAlloc.h>
#include <System/Numerics/Align.h>

namespace System::MemoryService {
    using Concurrent::ScopedLock;

    // MemoryZone s_defaultBuddyZone;

    Property<usize>         SpannedPages;
    Property<Atomic<isize>> ManagedPages;
    Property<usize>         PresentPages;
    Spinlock::Core          LockField;

    Array<PageQueue, sc_MaxQueueLevel + 1> PageQueues;

    PageQueue* FindQueueByLevel(u64 amount, u8& pageLevel)
    {
        RequiresOrNull(pageLevel < sc_MaxQueueLevel);
        while (pageLevel < sc_MaxQueueLevel) {
            if ((1 << pageLevel) >= amount &&
                PageQueues[pageLevel].Count != 0) {
                break;
            }
            pageLevel++;
        }
        return &PageQueues[pageLevel];
    }

    PageFrame* AllocatePhysFrame4K(usize amount)
    {
        RequiresOrNull(amount != 0 and amount <= sc_PageLengthLimit);
        ScopedLock lock(LockField);
        Numerics::AlignExpo2(amount);

        u8         pageLevel = 0;
        PageQueue* pageQueue = FindQueueByLevel(amount, pageLevel);
        PageFrame* pageFrame = (*pageQueue)();

        while ((1 << pageLevel) > amount and pageLevel != 0) {
            pageLevel--;

            auto* rest = pageFrame->GetAsSplit();
            RequiresOrNull(rest != nullptr);
            PageQueues[pageLevel] += rest;
        }

        pageFrame->_flags -= Hal::PmmFlags::READY;
        PresentPages -= amount;

        return pageFrame;
    }

    uptr AllocatePhysMemory4K(usize amount)
    {
        auto* ret = AllocatePhysFrame4K(amount);
        return ret ? ret->_address : 0;
    }

    void FreePhysFrame4K(PageFrame* page)
    {
        Requires(page != nullptr);
        if (page->_flags bitand (Hal::PmmFlags::READY |           //
                                 Hal::PmmFlags::KERNEL_RESERVED | //
                                 Hal::PmmFlags::PAGE_SWAPPED)) {
            return;
        }
        ScopedLock lock(LockField);

        page->_flags += Hal::PmmFlags::READY;
        while (page->_level < sc_MaxQueueLevel) {
            /*
                Merge the page until it reaches the upper bound
                or the another page needed is not free, then jump
                out of the loop
             */
            uptr  offset  = GetLevelAsOffset(page->_level);
            uptr  address = (page->_address % (offset * 2)) != 0
                                ? page->_address - offset
                                : page->_address + offset;
            auto* rest    = GetAsPageFrame(address);

            if (rest->_flags[Hal::PmmFlags::READY]) {
                if (rest->_previous || rest->_next) {
                    PageQueues[rest->_level] -= rest;
                }

                auto* result = page->GetAsCombined(rest);
                if (result != nullptr) {
                    page = result;
                    continue;
                }
            } else
                break;
        }

        PageQueues[page->_level] += page;
        return;
    }

    void FreePhysMemory4K(uptr address) {}

    void FreePhysMemory4K(uptr address, usize size) {}

    u64 MarkRegionAsUsed(MemoryRegion region)
    {
        RequiresOrZero(region.Length() != 0);
        region.GetAsInnerAligned(PAGE_SIZE_4K);
    }

    u64 MarkRegionAsFree(MemoryRegion region)
    {
        RequiresOrZero(
            region.GetAsRelocated(sc_pageBiosReserved)
                .GetAsInnerAligned(PAGE_SIZE_4K)
                .Length() != 0);
        ScopedLock lock(LockField);
        // info
        usize      pageLevelAssumption = sc_MaxQueueLevel;
        PageFrame* pageFrame{ nullptr };

        usize count{ 0 };
        usize amount = region.Length() / PAGE_SIZE_4K;
        PresentPages += amount;
        ManagedPages += amount;

        uptr address = (uptr)region._begin;
        uptr offset  = 0;
        while (address < region._end) {
            while ((offset = GetLevelAsOffset(pageLevelAssumption)) >
                   (region._end - address)) {
                pageLevelAssumption--;
            }

            pageFrame = GetAsPageFrame(address);
            if (!pageFrame->_address and
                !(pageFrame->_flags[Hal::PmmFlags::READY])) {
                *pageFrame = { ._flags   = Hal::PmmFlags::READY,
                               ._level   = 1,
                               ._address = address };
            }

            PageQueues[pageLevelAssumption] += pageFrame;
            count += (1 << pageLevelAssumption);
            address += offset;
        }

        return count;
    }
}