#pragma once

#include <System/Concurrent/Atomic.h>
#include <System/Concurrent/Spinlock.h>
#include <Types.h>

namespace System::Concurrent {
    static inline bool checkInterrupts()
    {
#if defined(__i386__) || defined(__x86_64__)
        volatile u64 flags;
        asm volatile(
            "pushfq;"
            "pop %0;"
            : "=rm"(flags)::"memory", "cc");
        return flags & 0x200;
#elif defined(__arm__)
        return false;
#endif
    }

    template <bool disableInterrupts = false>
    class ScopedLock final
    {
    public:
        always_inline inline ScopedLock(Spinlock::Core& lock)
            : m_lock(lock)
        {
            if constexpr (disableInterrupts) {
                m_irq = checkInterrupts();
                if (m_irq) {
                    asm volatile("cli");
                    while (lock.xchg(1, MemoryOrder::ACQUIRE))
                        asm volatile("sti; pause; cli");
                } else {
                    while (lock.xchg(1, MemoryOrder::ACQUIRE))
                        asm volatile("pause");
                }
            } else {
                while (lock.xchg(1, MemoryOrder::ACQUIRE))
                    asm volatile("pause");
            }
        }

        always_inline inline ~ScopedLock()
        {
            m_lock.store(0, MemoryOrder::RELEASE);

            if constexpr (disableInterrupts) {
                if (m_irq)
                    asm volatile("sti");
            }
        }

    private:
        Spinlock::Core& m_lock;
        bool            m_irq{ false };
    };
}