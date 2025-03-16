#pragma once

#include <TypeTraits.h>
#include <Types.h>
#include <Utility.h>

namespace System::Concurrent {
    enum class MemoryOrder : int
    {
        RELAXED = __ATOMIC_RELAXED,
        CONSUME = __ATOMIC_CONSUME,
        ACQUIRE = __ATOMIC_ACQUIRE,
        RELEASE = __ATOMIC_RELEASE,
        ACQ_REL = __ATOMIC_ACQ_REL,
        SEQ_CST = __ATOMIC_SEQ_CST
    };

    constexpr static inline u32 MO_MASK          = 0x0FFFF;
    constexpr static inline u32 MO_MASK_MODIFIER = 0xFFFF0000;
    constexpr static inline u32 MO_HLE_ACQUIRE   = 0x10000;
    constexpr static inline u32 MO_HLE_RELEASE   = 0x20000;

    template <typename T>
    class Atomic
    {
    public:
        constexpr Atomic()
            requires Std::isDefaultConstructible<T>
            : m_value{}
        {
        }

        constexpr Atomic(T const& value)
            requires Std::isCopyConstructible<T>
            : m_value{ value }
        {
        }

        constexpr Atomic(T&& value)
            requires Std::isMoveConstructible<T>
            : m_value{ Std::move(value) }
        {
        }

        always_inline T
        xchg(T const& value, MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline bool cmpxchg(
            T const&    expected,
            T const&    desired,
            MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline T
        fetchAdd(T val, MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline T
        fetchSub(T val, MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline T
        fetchAnd(T val, MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline T
        fetchOr(T val, MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline T
        fetchXor(T val, MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline T
        fetchNand(T val, MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline T fetchInc(MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline T fetchDec(MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline void inc(MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline void dec(MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline T load(MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline void store(
            T const&    value,
            MemoryOrder order = MemoryOrder::SEQ_CST);

        always_inline T operator+=(T val) { return fetchAdd(val); }

        always_inline T operator-=(T val) { return fetchSub(val); }

        always_inline T operator&=(T val) { return fetchAnd(val); }

        always_inline T operator|=(T val) { return fetchOr(val); }

        always_inline T operator^=(T val) { return fetchXor(val); }

        always_inline T operator++() { return fetchInc(); }

        always_inline T operator++(int) { return fetchInc() - 1; }

        always_inline T operator--() { return fetchDec(); }

        always_inline T operator--(int) { return fetchDec() + 1; }

        always_inline void operator=(T const& value)
        {
            store(value);
            return value;
        }

        always_inline T operator()() { return load(); }

        always_inline bool isLockFree() const;

    private:
        T m_value;
    };

    template <typename T>
    always_inline T Atomic<T>::xchg(T const& value, MemoryOrder order)
    {
        return __atomic_exchange_n(&m_value, value, static_cast<int>(order));
    }

    template <typename T>
    always_inline bool
    Atomic<T>::cmpxchg(T const& expected, T const& desired, MemoryOrder order)
    {
        if (order == MemoryOrder::ACQ_REL || order == MemoryOrder::RELAXED) {
            return __atomic_compare_exchange_n(
                &m_value,
                &expected,
                desired,
                false,
                MemoryOrder::RELAXED,
                MemoryOrder::ACQUIRE);
        }
        return __atomic_compare_exchange_n(
            &m_value,
            &expected,
            desired,
            false,
            static_cast<int>(order),
            static_cast<int>(order));
    }

    template <typename T>
    always_inline T Atomic<T>::fetchAdd(T val, MemoryOrder order)
    {
        return __atomic_fetch_add(&m_value, val, static_cast<int>(order));
    }

    template <typename T>
    always_inline T Atomic<T>::fetchSub(T val, MemoryOrder order)
    {
        return __atomic_fetch_sub(&m_value, val, static_cast<int>(order));
    }

    template <typename T>
    always_inline T Atomic<T>::fetchAnd(T val, MemoryOrder order)
    {
        return __atomic_fetch_and(&m_value, val, static_cast<int>(order));
    }

    template <typename T>
    always_inline T Atomic<T>::fetchOr(T val, MemoryOrder order)
    {
        return __atomic_fetch_or(&m_value, val, static_cast<int>(order));
    }

    template <typename T>
    always_inline T Atomic<T>::fetchXor(T val, MemoryOrder order)
    {
        return __atomic_fetch_xor(&m_value, val, static_cast<int>(order));
    }

    template <typename T>
    always_inline T Atomic<T>::fetchNand(T val, MemoryOrder order)
    {
        return __atomic_fetch_nand(&m_value, val, static_cast<int>(order));
    }

    template <typename T>
    always_inline T Atomic<T>::fetchInc(MemoryOrder order)
    {
        return fetchAdd(1, order);
    }

    template <typename T>
    always_inline T Atomic<T>::fetchDec(MemoryOrder order)
    {
        return fetchSub(1, order);
    }

    template <typename T>
    always_inline void Atomic<T>::inc(MemoryOrder order)
    {
        fetchInc(order);
    }

    template <typename T>
    always_inline void Atomic<T>::dec(MemoryOrder order)
    {
        fetchDec(order);
    }

    template <typename T>
    always_inline T Atomic<T>::load(MemoryOrder order)
    {
        return __atomic_load_n(&m_value, static_cast<int>(order));
    }

    template <typename T>
    always_inline void Atomic<T>::store(T const& value, MemoryOrder order)
    {
        __atomic_store_n(&m_value, value, static_cast<int>(order));
    }

    template <typename T>
    always_inline bool Atomic<T>::isLockFree() const
    {
        return __atomic_is_lock_free(sizeof(T), &m_value);
    }
}