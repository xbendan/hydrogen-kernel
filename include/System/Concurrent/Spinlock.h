#pragma once

#include <System/Concurrent/Atomic.h>

namespace System::Concurrent {
    class Spinlock
    {
    public:
        using Core = Atomic<int>;

        void operator=(bool const& value);
    };
}