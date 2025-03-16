#pragma once

#include <System/Memory/PageFrame.h>

namespace System::MemoryService {
    class PageQueue
    {
    public:
        void       operator+=(PageFrame* region);
        void       operator-=(PageFrame* region);
        PageFrame* operator()();

        ReadonlyProperty<u32, PageQueue> Count;

    protected:
        PageFrame *m_head, *m_tail;
    };
    static_assert(sizeof(PageQueue) == 24, "PageQueue size mismatch");
}