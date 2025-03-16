#pragma once

#include <System/Concurrent/Spinlock.h>
#include <System/Memory/PageFrame.h>
#include <System/Memory/PageQueue.h>
#include <System/Text/StringView.h>

namespace System::MemoryService {
    using Concurrent::Spinlock;

    // struct KernelProcessorCache
    // {
    //     /*
    //      * Pointer to the next object, this is a double pointer
    //      * because the area this pointer point to is also a pointer
    //      * to the next object.
    //      */
    //     // void**     _freelist;
    //     PageFrame* _page;
    //     PageQueue  _partial;
    // };

    struct KernelProcessorCache
    {
        struct
        {
            void**        Freelist;
            unsigned long TransactionID;
        };
        Spinlock::Core Lock;
        PageFrame*     Inuse;
        PageQueue      Partial;
    };

    struct KernelCacheGeneric
    {
        constexpr KernelCacheGeneric(u64 size, u64 flags = 0)
            : _size(size)
            , _objSize(size)
            , _objAlign(0)
            , _reserved(PAGE_SIZE_4K % size)
        {
        }
        ~KernelCacheGeneric() = default;

        strview Name;
        // KernelProcessorCache* ProcessorCache;

        SlabAllocNode _fallback;
        lock_t        _lock;
        u32           _size;
        u32           _objSize;
        u32           _objAlign;
        u32           _offset;
        u64           _minPartial;
        u32           _reserved;
    };
}