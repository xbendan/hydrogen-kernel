#pragma once

#include <Meta/Flags.h>
#include <Meta/Tuple.h>
#include <System/Collections/LinkedList.h>
#include <System/Hal/MemoryFlags.h>
#include <System/Memory/MemoryRegion.h>

namespace System::MemoryService {
    using Collections::LinkedList;

    constexpr inline usize PAGE_SIZE_4K = 4096;
    constexpr inline usize PAGE_SIZE_2M = 2097152;
    constexpr inline usize PAGE_SIZE_1G = 1073741824;

    struct PageFrame : public LinkedList<>::Linker<PageFrame>
    {
        Flags<Hal::PmmFlags> _flags;
        u8                   _level;
        u16                  _chainLength;

        struct
        {
            struct
            {
                u32 _inuse : 16;
                u32 _objects : 15;
                u32 _frozen : 1;
            } _kmem __attribute__((packed));
            void** _pointers;
        };

        union
        {
            qword      _priv;
            void*      _kmemPool;
            PageFrame* _pageHead;
        };

        uptr _address;

        always_inline constexpr void operator=(
            Tuple<Flags<Hal::PmmFlags>, u8, uptr> data);

        MemoryRegion GetAsMemoryRegion();
        PageFrame*   GetAsCombined(PageFrame* other);
        PageFrame*   GetAsSplit();
    };

    constexpr inline PageFrame* GetAsPageFrame(u64 address) {}

    static inline bool isPageFrameHeaded( //
        u64 address,
        u8  level,
        u64 size = PAGE_SIZE_4K)
    {
        return !(address % ((1 << level) * size));
    }

    static inline u8 GetSizeAsLevel(usize size)
    {
        u8 _ = 0;
        while (size >>= 1) {
            _++;
        }
        return _;
    }

    static inline usize GetLevelAsSize(u8 level)
    {
        return 1 << level;
    }

    static inline u64 GetLevelAsOffset(u8 level, usize offset = PAGE_SIZE_4K)
    {
        return GetLevelAsSize(level) * offset;
    }

    PageFrame* GetFrameAsCombined(PageFrame* lhs, PageFrame* rhs);
    PageFrame* GetFrameAsCombined(PageFrame* frame);
    PageFrame* GetFrameAsSplit(PageFrame* frame);
}