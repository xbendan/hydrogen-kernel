#pragma once

#include <Meta/Array.h>
#include <System/Concurrent/Atomic.h>
#include <System/Memory/MemoryRegion.h>
#include <System/Memory/PageQueue.h>

namespace System::MemoryService {
    using Concurrent::Atomic;

    struct MemoryZone
    {
        MemoryRegion m_region;
        u64          m_pfnBegin, m_pfnEnd;

        /*
         * spanned_pages is the total pages spanned by the zone, including
         * holes, which is calculated as:
         * 	spanned_pages = zone_end_pfn - zone_start_pfn;
         */
        Property<u64>                             SpannedPages;
        /*
         * managed_pages is present pages managed by the buddy system, which
         * is calculated as (reserved_pages includes pages allocated by the
         * bootmem allocator):
         *	managed_pages = present_pages - reserved_pages;
         */
        ReadonlyProperty<Atomic<i64>, MemoryZone> ManagedPages;
        // Atomic<long>                  m_managedPages;
        /*
         * present_pages is physical pages existing within the zone, which
         * is calculated as:
         *	present_pages = spanned_pages - absent_pages(pages in holes);
         *
         * So present_pages may be used by memory hotplug or memory power
         * management logic to figure out unmanaged pages by checking
         * (present_pages - managed_pages). And managed_pages should be used
         * by page allocator and vm scanner to calculate all kinds of
         * watermarks and thresholds.
         */
        Property<usize>                           PresentPages;
        // u64                                       m_presentPages;

        Atomic<int> m_lock;

        MemoryRegion GetAsMemoryRegion() const;

        PageQueue* FindQueue(usize amount, u8& level);

        void operator+=(MemoryRegion region);
        void operator-=(MemoryRegion region);

        usize IncludeRegionAsFree(MemoryRegion region);
        usize ExcludeRegionAsUsed(MemoryRegion region);

        PageQueue& operator[](u8 level);

    private:
        // PageQueueSet m_pageQueues;
    };
}