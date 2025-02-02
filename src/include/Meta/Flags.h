#include <StdC++/TypeTraits.h>

namespace Meta {

#define MakeFlags$(T)                                                          \
    constexpr inline T operator~(T rhs)                                        \
    {                                                                          \
        using U = Std::UnderlyingType<T>;                                      \
        return (T) ~(U)rhs;                                                    \
    }                                                                          \
    constexpr inline T operator|(T lhs, T rhs)                                 \
    {                                                                          \
        using U = Std::UnderlyingType<T>;                                      \
        return (T)((U)lhs | (U)rhs);                                           \
    }                                                                          \
    constexpr inline T operator&(T lhs, T rhs)                                 \
    {                                                                          \
        using U = Std::UnderlyingType<T>;                                      \
        return (T)((U)lhs & (U)rhs);                                           \
    }                                                                          \
    constexpr inline T operator^(T lhs, T rhs)                                 \
    {                                                                          \
        using U = Std::UnderlyingType<T>;                                      \
        return (T)((U)lhs ^ (U)rhs);                                           \
    }                                                                          \
    constexpr inline T& operator|=(T& lhs, T rhs)                              \
    {                                                                          \
        return lhs = lhs | rhs;                                                \
    }                                                                          \
    constexpr inline T& operator&=(T& lhs, T rhs)                              \
    {                                                                          \
        return lhs = lhs & rhs;                                                \
    }                                                                          \
    constexpr inline T& operator^=(T& lhs, T rhs)                              \
    {                                                                          \
        return lhs = lhs ^ rhs;                                                \
    }

    template <typename E>
        requires Std::isEnum<E>
    struct Flags
    {
        using EnumType       = E;
        using UnderlyingType = Std::UnderlyingType<E>;

    public:
        constexpr Flags() noexcept = default;

        constexpr Flags(UnderlyingType value) noexcept
            : m_value(value)
        {
        }

        constexpr Flags(EnumType value) noexcept
            : m_value(static_cast<UnderlyingType>(value))
        {
        }

        constexpr Flags(const Flags& other) noexcept = default;

        constexpr Flags(Flags&& other) noexcept = default;

        constexpr Flags& operator=(const Flags& other) noexcept = default;

        constexpr Flags& operator=(Flags&& other) noexcept = default;

        constexpr Flags operator|(Flags other) const noexcept
        {
            return Flags(m_value | other.m_value);
        }

        constexpr Flags operator&(Flags other) const noexcept
        {
            return Flags(m_value & other.m_value);
        }

        constexpr Flags operator^(Flags other) const noexcept
        {
            return Flags(m_value ^ other.m_value);
        }

        constexpr Flags operator~() const noexcept { return Flags(~m_value); }

        constexpr Flags& operator|=(Flags other) noexcept
        {
            m_value |= other.m_value;
            return *this;
        }

        constexpr Flags& operator&=(Flags other) noexcept
        {
            m_value &= other.m_value;
            return *this;
        }

        constexpr Flags& operator^=(Flags other) noexcept
        {
            m_value ^= other.m_value;
            return *this;
        }

        constexpr bool operator==(Flags other) const noexcept
        {
            return m_value == other.m_value;
        }

        constexpr bool operator!=(Flags other) const noexcept
        {
            return m_value != other.m_value;
        }

        constexpr explicit operator UnderlyingType() const noexcept
        {
            return m_value;
        }

        constexpr explicit operator bool() const noexcept
        {
            return m_value != 0;
        }

        constexpr bool operator!() const noexcept { return m_value == 0; }

        constexpr UnderlyingType operator*() const noexcept { return m_value; }

        constexpr bool operator[](EnumType flag) const noexcept
        {
            return (m_value & static_cast<UnderlyingType>(flag)) != 0;
        }

        constexpr void operator+=(EnumType flag) noexcept
        {
            m_value |= static_cast<UnderlyingType>(flag);
        }

        constexpr void operator-=(EnumType flag) noexcept
        {
            m_value &= ~static_cast<UnderlyingType>(flag);
        }

        constexpr void operator+=(Flags other) noexcept
        {
            m_value |= other.m_value;
        }

        constexpr void operator-=(Flags other) noexcept
        {
            m_value &= ~other.m_value;
        }

    private:
        UnderlyingType m_value = 0;
    };
}

using Meta::Flags;