#pragma once

#include <Concepts.h>
#include <Types.h>

namespace Meta {
    struct PublicAccess
    {};

    template <typename... Ts>
    struct FriendAccess
    {};

    template <typename T, typename... Ts>
    concept AccessModifier = Included<T, PublicAccess, FriendAccess<Ts...>>;

    template <typename T>
    T& _Getter(T& field)
    {
        return field;
    }

    template <typename T>
    void _Setter(T& field, T& value)
    {
        field = Std::move(value);
    }
}

using Meta::AccessModifier;
using Meta::FriendAccess;
using Meta::PublicAccess;

template <
    typename T,
    typename TAccessor                   = Empty,
    AccessModifier<TAccessor>  GetAccess = PublicAccess,
    AccessModifier<TAccessor>  SetAccess = PublicAccess,
    auto                       Getter    = Meta::_Getter<T>,
    decltype(Meta::_Setter<T>) Setter    = Meta::_Setter<T>>
class Property
{
public:
    always_inline constexpr Property() = default;
    always_inline constexpr Property(T const& value)
        : m_value(value)
    {
    }
    // always_inline constexpr Property(T& value)
    //     : m_value(Std::move(value))
    // {
    // }

    Property(Property const&) = delete;
    Property(Property&&)      = delete;

    always_inline constexpr Property& operator=(T& value)
        requires(Std::isSame<SetAccess, PublicAccess>)
    {
        Setter(m_value, value);

        return *this;
    }

    always_inline constexpr operator T const&() const
        requires(Std::isSame<GetAccess, PublicAccess>)
    {
        return Getter(m_value);
    }

    always_inline constexpr operator T&()
        requires(Std::isSame<GetAccess, PublicAccess>)
    {
        return Getter(m_value);
    }

    always_inline constexpr T* operator->()
        requires(Std::isSame<GetAccess, PublicAccess>)
    {
        return &Getter(m_value);
    }

    always_inline constexpr T const* operator->() const
        requires(Std::isSame<GetAccess, PublicAccess>)
    {
        return &Getter(m_value);
    }

    template <typename T1>
    always_inline void operator+=(T1 value)
        requires requires { declval<T>() += declval<T1>(); }
    {
        m_value += value;
    }

    template <typename T1>
    always_inline void operator-=(T1 value)
        requires requires { declval<T>() -= declval<T1>(); }
    {
        m_value -= value;
    }

    template <typename T1>
    always_inline void operator*=(T1 value)
        requires requires { declval<T>() *= declval<T1>(); }
    {
        m_value *= value;
    }

    template <typename T1>
    always_inline void operator/=(T1 value)
        requires requires { declval<T>() /= declval<T1>(); }
    {
        m_value /= value;
    }

protected:
    friend TAccessor;

    Property& operator=(T& value)
        requires(Std::isSame<SetAccess, FriendAccess<TAccessor>>)
    {
        Setter(m_value, value);

        return *this;
    }

    operator T&()
        requires(Std::isSame<GetAccess, FriendAccess<TAccessor>>)
    {
        return Getter(m_value);
    }

private:
    T m_value;
};

template <typename T, typename TAccessor>
using ReadonlyProperty =
    Property<T, TAccessor, PublicAccess, FriendAccess<TAccessor>>;

template <typename T, auto Setter, auto Getter>
using PropertyAccessors =
    Property<T, Empty, PublicAccess, PublicAccess, Getter, Setter>;

template <typename T>
class DependencyProperty
{
public:
    always_inline constexpr DependencyProperty() = default;
    always_inline constexpr DependencyProperty(T& value)
        : m_value(value)
    {
    }

    DependencyProperty(DependencyProperty const&) = delete;
    DependencyProperty(DependencyProperty&&)      = delete;

    always_inline constexpr DependencyProperty& operator=(T& value)
    {
        m_value = value;

        return *this;
    }

    always_inline constexpr operator T const&() const { return m_value; }

    always_inline operator T&() { return m_value; }

    always_inline constexpr T* operator->() { return &m_value; }

    always_inline constexpr T const* operator->() const { return &m_value; }

    template <typename T1>
    always_inline void operator+=(T1 value)
        requires requires { declval<T>() += declval<T1>(); }
    {
        m_value += value;
    }

    template <typename T1>
    always_inline void operator-=(T1 value)
        requires requires { declval<T>() -= declval<T1>(); }
    {
        m_value -= value;
    }

    template <typename T1>
    always_inline void operator*=(T1 value)
        requires requires { declval<T>() *= declval<T1>(); }
    {
        m_value *= value;
    }

    template <typename T1>
    always_inline void operator/=(T1 value)
        requires requires { declval<T>() /= declval<T1>(); }
    {
        m_value /= value;
    }

private:
    T& m_value;
};