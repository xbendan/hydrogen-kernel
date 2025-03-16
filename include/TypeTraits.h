#pragma once

#include <Decl.h>
#include <Types.h>

namespace Std {
    template <typename T, T t>
    struct Constant
    {
        static constexpr T value = t;
        using ValueType          = T;
        using Type               = Constant<T, t>;

        constexpr operator ValueType() const noexcept { return value; }
        constexpr ValueType operator()() const noexcept { return value; }
    };

    using TrueType  = Constant<bool, true>;
    using FalseType = Constant<bool, false>;

    template <typename T>
    struct TypeIdentity
    {
        using Type = T;
    };

    template <typename TFrom, typename TTo>
    struct _isChildTypeOf : Constant<bool, __is_base_of(TFrom, TTo)>
    {};

    template <typename TFrom, typename TTo>
    constexpr inline bool isChildTypeOf = _isChildTypeOf<TFrom, TTo>::value;

    template <typename T, unsigned = sizeof(T)>
    constexpr TrueType isCompleteOrUnbounded(TypeIdentity<T>) noexcept
    {
        return TrueType();
    }

    // MARK: - Arithmetic

    template <typename T>
    struct _isSigned : Constant<bool, T(-1) < T(0)>
    {};

    template <typename T>
    constexpr inline bool isSigned = _isSigned<T>::value;

    template <typename T>
    struct _isUnsigned : Constant<bool, T(0) < T(-1)>
    {};

    template <typename T>
    constexpr inline bool isUnsigned = _isUnsigned<T>::value;

    template <typename T>
    struct _isIntegral : FalseType
    {};

    template <>
    struct _isIntegral<u8> : TrueType
    {};

    template <>
    struct _isIntegral<u16> : TrueType
    {};

    template <>
    struct _isIntegral<u32> : TrueType
    {};

    template <>
    struct _isIntegral<u64> : TrueType
    {};

    template <>
    struct _isIntegral<usize> : TrueType
    {};

    template <>
    struct _isIntegral<i8> : TrueType
    {};

    template <>
    struct _isIntegral<i16> : TrueType
    {};

    template <>
    struct _isIntegral<i32> : TrueType
    {};

    template <>
    struct _isIntegral<i64> : TrueType
    {};

    template <typename T>
    constexpr inline bool isIntegral = _isIntegral<T>::value;

    template <typename T>
    struct _isFloatingPoint : FalseType
    {};

    template <>
    struct _isFloatingPoint<f32> : TrueType
    {};

    template <>
    struct _isFloatingPoint<f64> : TrueType
    {};

    template <>
    struct _isFloatingPoint<f128> : TrueType
    {};

    template <typename T>
    constexpr inline bool isFloatingPoint = _isFloatingPoint<T>::value;

    template <typename T>
    struct _isArithmetic : Constant<bool, isIntegral<T> || isFloatingPoint<T>>
    {};

    template <typename T>
    constexpr inline bool isArithmetic = _isArithmetic<T>::value;

    // MARK: - Compound

    template <typename T>
    struct _isCompound : Constant<bool, !isArithmetic<T>>
    {};

    // MARK: - Void

    template <typename T>
    struct _isVoid : Constant<bool, __is_void(T)>
    {};

    template <typename T>
    constexpr inline bool isVoid = _isVoid<T>::value;

    // MARK: - Cast

    namespace _ {
        template <class T>
        auto tReturnable(int)
            -> decltype(void(static_cast<T (*)()>(nullptr)), TrueType{});
        template <class>
        auto tReturnable(...) -> FalseType;

        template <class From, class To>
        auto tImplicitlyConvertible(int)
            -> decltype(void(declval<void (&)(To)>()(declval<From>())), TrueType{});

        template <class, class>
        auto tImplicitlyConvertible(...) -> FalseType;
    }

    template <typename From, typename To>
    struct _isConvertible
        : Constant<
              bool,
              (decltype(_::tReturnable<To>(0))::value &&
               decltype(_::tImplicitlyConvertible<From, To>(0))::value) ||
                  (isVoid<From> && isVoid<To>)>
    {};

    template <typename From, typename To>
    inline constexpr bool isConvertible = _isConvertible<From, To>::value;

    template <typename T0, typename T1>
    struct _isSame : FalseType
    {};

    template <typename T>
    struct _isSame<T, T> : TrueType
    {};

    template <typename T0, typename T1>
    constexpr inline bool isSame = _isSame<T0, T1>::value;

    template <typename T>
    constexpr inline bool isCompound = _isCompound<T>::value;

    // MARK: - Const & Volatile

    template <typename T>
    struct _isConst : FalseType
    {};

    template <typename T>
    struct _isConst<const T> : TrueType
    {};

    template <typename T>
    constexpr inline bool isConst = _isConst<T>::value;

    template <typename T>
    struct _isVolatile : FalseType
    {};

    template <typename T>
    struct _isVolatile<volatile T> : TrueType
    {};

    template <typename T>
    constexpr inline bool isVolatile = _isVolatile<T>::value;

    template <typename T>
    struct _isConstVolatile : Constant<bool, isConst<T> || isVolatile<T>>
    {};

    template <typename T>
    constexpr inline bool isConstVolatile = _isConstVolatile<T>::value;

    template <typename T>
    struct _toConstType
    {
        using Type = const T;
    };

    template <typename T>
    struct _toConstType<const T>
    {
        using Type = const T;
    };

    template <typename T>
    struct _toConstType<volatile T>
    {
        using Type = const volatile T;
    };

    template <typename T>
    struct _toConstType<const volatile T>
    {
        using Type = const volatile T;
    };

    template <typename T>
    using ConstType = typename _toConstType<T>::Type;

    template <typename T>
    struct _toVolatileType
    {
        using Type = volatile T;
    };

    template <typename T>
    struct _toVolatileType<const T>
    {
        using Type = const volatile T;
    };

    template <typename T>
    struct _toVolatileType<volatile T>
    {
        using Type = volatile T;
    };

    template <typename T>
    struct _toVolatileType<const volatile T>
    {
        using Type = const volatile T;
    };

    template <typename T>
    using VolatileType = typename _toVolatileType<T>::Type;

    template <typename T>
    struct _toConstVolatileType
    {
        using Type = const volatile T;
    };

    template <typename T>
    struct _toConstVolatileType<const T>
    {
        using Type = const volatile T;
    };

    template <typename T>
    struct _toConstVolatileType<volatile T>
    {
        using Type = const volatile T;
    };

    template <typename T>
    struct _toConstVolatileType<const volatile T>
    {
        using Type = const volatile T;
    };

    template <typename T>
    using CvType = typename _toConstVolatileType<T>::Type;

    template <typename T>
    struct _RemoveConst
    {
        using Type = T;
    };

    template <typename T>
    struct _RemoveConst<const T>
    {
        using Type = T;
    };

    template <typename T>
    struct _RemoveConst<volatile T>
    {
        using Type = volatile T;
    };

    template <typename T>
    struct _RemoveConst<const volatile T>
    {
        using Type = volatile T;
    };

    template <typename T>
    using RemoveConst = typename _RemoveConst<T>::Type;

    template <typename T>
    struct _RemoveVolatile
    {
        using Type = T;
    };

    template <typename T>
    struct _RemoveVolatile<const T>
    {
        using Type = const T;
    };

    template <typename T>
    struct _RemoveVolatile<volatile T>
    {
        using Type = T;
    };

    template <typename T>
    struct _RemoveVolatile<const volatile T>
    {
        using Type = const T;
    };

    template <typename T>
    using RemoveVolatile = typename _RemoveVolatile<T>::Type;

    template <typename T>
    struct _RemoveCv
    {
        using Type = T;
    };

    template <typename T>
    struct _RemoveCv<const T>
    {
        using Type = T;
    };

    template <typename T>
    struct _RemoveCv<volatile T>
    {
        using Type = T;
    };

    template <typename T>
    struct _RemoveCv<const volatile T>
    {
        using Type = T;
    };

    template <typename T>
    using RemoveCv = typename _RemoveCv<T>::Type;

    // MARK: - Reference

    template <typename T>
    struct _isReference : FalseType
    {};

    template <typename T>
    struct _isReference<T&> : TrueType
    {};

    template <typename T>
    struct _isReference<T&&> : TrueType
    {};

    template <typename T>
    constexpr inline bool isReference = _isReference<T>::value;

    template <typename T>
    struct _isLvalueReference : FalseType
    {};

    template <typename T>
    struct _isLvalueReference<T&> : TrueType
    {};

    template <typename T>
    constexpr inline bool isLvalueReference = _isLvalueReference<T>::value;

    template <typename T>
    struct _isRvalueReference : FalseType
    {};

    template <typename T>
    struct _isRvalueReference<T&&> : TrueType
    {};

    template <typename T>
    constexpr inline bool isRvalueReference = _isRvalueReference<T>::value;

    template <typename T>
    struct _RemoveRef
    {
        using Type = T;
    };

    template <typename T>
    struct _RemoveRef<T&>
    {
        using Type = T;
    };

    template <typename T>
    struct _RemoveRef<T&&>
    {
        using Type = T;
    };

    template <typename T>
    using RemoveRef = typename _RemoveRef<T>::Type;

    template <typename T>
    using RemoveCvRef = RemoveCv<RemoveRef<T>>;

    template <typename T>
    struct _LvalueRef
    {
        using Type = T&;
    };

    template <typename T>
    struct _LvalueRef<T&>
    {
        using Type = T&;
    };

    template <typename T>
    struct _LvalueRef<T&&>
    {
        using Type = T&;
    };

    template <typename T>
    using LvalueRef = typename _LvalueRef<T>::Type;

    template <typename T>
    struct _RvalueRef
    {
        using Type = T&&;
    };

    template <typename T>
    struct _RvalueRef<T&>
    {
        using Type = T&&;
    };

    template <typename T>
    struct _RvalueRef<T&&>
    {
        using Type = T&&;
    };

    template <typename T>
    using RvalueRef = typename _RvalueRef<T>::Type;

    // MARK: - Pointer

    template <typename T>
    struct _isPointer : FalseType
    {};

    template <typename T>
    struct _isPointer<T*> : TrueType
    {};

    template <typename T>
    constexpr inline bool isPointer = _isPointer<T>::value;

    template <typename T>
    struct _isMemberPointer : FalseType
    {};

    template <typename T, typename U>
    struct _isMemberPointer<T U::*> : TrueType
    {};

    template <typename T>
    constexpr inline bool isMemberPointer = _isMemberPointer<T>::value;

    template <typename T>
    struct _Pointed
    {
        using Type = T*;
    };

    template <typename T>
    struct _Pointed<T*>
    {
        using Type = T*;
    };

    template <typename T>
    using Pointed = typename _Pointed<T>::Type;

    // MARK: - Function

    template <typename T>
    struct _isFunction : FalseType
    {};

    template <typename R, typename... Args>
    struct _isFunction<R(Args...)> : TrueType
    {};

    template <typename R, typename... Args>
    constexpr inline bool isFunction = _isFunction<R(Args...)>::value;

    // MARK: - Class

    template <typename T>
    struct _isClass : Constant<bool, __is_class(T)>
    {};

    template <typename T>
    constexpr inline bool isClass = _isClass<T>::value;

    // MARK: - Union

    template <typename T>
    struct _isUnion : Constant<bool, __is_union(T)>
    {};

    template <typename T>
    constexpr inline bool isUnion = _isUnion<T>::value;

    // MARK: - Enum

    template <typename T>
    struct _isEnum : Constant<bool, __is_enum(T)>
    {};

    template <typename T>
    constexpr inline bool isEnum = _isEnum<T>::value;

    template <typename T>
    struct _isScopedEnum : Constant<bool, __is_scoped_enum(T)>
    {};

    template <typename T>
    constexpr inline bool isScopedEnum = _isScopedEnum<T>::value;

    template <typename T>
        requires isEnum<T>
    struct _UnderlyingType
    {
        using Type = __underlying_type(T);
    };

    template <typename T>
    using UnderlyingType = typename _UnderlyingType<T>::Type;

    template <typename T>
    constexpr UnderlyingType<T> getAsUnderlyingType(T value)
    {
        return static_cast<UnderlyingType<T>>(value);
    }

    // MARK: - Array

    template <typename T>
    struct _isArray : FalseType
    {};

    template <typename T>
    struct _isArray<T[]> : TrueType
    {};

    template <typename T, usize N>
    struct _isArray<T[N]> : TrueType
    {};

    template <typename T>
    constexpr inline bool isArray = _isArray<T>::value;

    template <typename T>
    struct _isBoundedArray : FalseType
    {};

    template <typename T, usize N>
    struct _isBoundedArray<T[N]> : TrueType
    {};

    template <typename T>
    constexpr inline bool isBoundedArray = _isBoundedArray<T>::value;

    template <typename T>
    struct _isUnboundedArray : FalseType
    {};

    template <typename T>
    struct _isUnboundedArray<T[]> : TrueType
    {};

    template <typename T>
    constexpr inline bool isUnboundedArray = _isUnboundedArray<T>::value;

    template <typename T>
    struct _makeItUnextent
    {
        using Type = T;
    };

    template <typename T>
    struct _makeItUnextent<T[]>
    {
        using Type = T;
    };

    template <typename T, usize N>
    struct _makeItUnextent<T[N]>
    {
        using Type = T;
    };

    template <typename T>
    using makeItUnextent = typename _makeItUnextent<T>::Type;

    // MARK: - Types

    template <typename T>
    struct _isFundamental : Constant<bool, isArithmetic<T> || isVoid<T>>
    {};

    template <typename T>
    constexpr inline bool isFundamental = _isFundamental<T>::value;

    template <typename T>
    struct _isScalar
        : Constant<
              bool,
              isArithmetic<T> || isPointer<T> || isMemberPointer<T> ||
                  isEnum<T> || isCompound<T>>
    {};

    template <typename T>
    constexpr inline bool isScalar = _isScalar<T>::value;

    template <typename T>
    struct _isTrivial : Constant<bool, __is_trivial(T)>
    {
        static_assert(
            isCompleteOrUnbounded(TypeIdentity<T>()),
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    struct _isObject
        : Constant<bool, isScalar<T> || isArray<T> || isUnion<T> || isClass<T>>
    {};

    template <typename T>
    constexpr inline bool isObject = _isObject<T>::value;

    template <typename T>
    constexpr inline bool isTrivial = _isTrivial<T>::value;

    template <typename T>
    struct _isTriviallyCopyable : Constant<bool, __is_trivially_copyable(T)>
    {
        static_assert(
            isCompleteOrUnbounded(TypeIdentity<T>()),
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    constexpr inline bool isTriviallyCopyable = _isTriviallyCopyable<T>::value;

    template <typename T>
    struct _isStandardLayout : Constant<bool, __is_standard_layout(T)>
    {
        static_assert(
            isCompleteOrUnbounded(TypeIdentity<T>()),
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    constexpr inline bool isStandardLayout = _isStandardLayout<T>::value;

    template <typename T>
    struct _isPOD : Constant<bool, __is_pod(T)>
    {
        static_assert(
            isCompleteOrUnbounded(TypeIdentity<T>()),
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    constexpr inline bool isPOD = _isPOD<T>::value;

    template <typename T>
    struct _isLiteral : Constant<bool, __is_literal_type(T)>
    {
        static_assert(
            isCompleteOrUnbounded(TypeIdentity<T>()),
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    constexpr inline bool isLiteral = _isLiteral<T>::value;

    template <typename T>
    struct _isEmpty : Constant<bool, __is_empty(T)>
    {
        static_assert(
            isCompleteOrUnbounded(TypeIdentity<T>()),
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    constexpr inline bool isEmpty = _isEmpty<T>::value;

    template <typename T>
    struct _isPolymorphic : Constant<bool, __is_polymorphic(T)>
    {
        static_assert(
            isCompleteOrUnbounded(TypeIdentity<T>()),
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    constexpr inline bool isPolymorphic = _isPolymorphic<T>::value;

    template <typename T>
    struct _isAbstract : Constant<bool, __is_abstract(T)>
    {
        static_assert(
            isCompleteOrUnbounded(TypeIdentity<T>()),
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    constexpr inline bool isAbstract = _isAbstract<T>::value;

    template <typename T>
    struct _isFinal : Constant<bool, __is_final(T)>
    {
        static_assert(
            isCompleteOrUnbounded(TypeIdentity<T>()),
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    constexpr inline bool isFinal = _isFinal<T>::value;

    template <typename T>
    struct _isAggregate : Constant<bool, __is_aggregate(T)>
    {
        static_assert(
            isCompleteOrUnbounded(TypeIdentity<T>()),
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    constexpr inline bool isAggregate = _isAggregate<T>::value;

    // MARK: Construct & Destruct

    template <typename T, typename... Args>
    struct _isConstructible : Constant<bool, __is_constructible(T, Args...)>
    {};

    template <typename T, typename... Args>
    constexpr inline bool isConstructible = _isConstructible<T, Args...>::value;

    template <typename T>
    constexpr inline bool isDefaultConstructible = isConstructible<T>;

    template <typename T>
    struct _isCopyConstructible
        : public _isConstructible<
              T,
              typename Std::LvalueRef<typename Std::ConstType<T>>>
    {};

    template <typename T>
    constexpr inline bool isCopyConstructible = _isCopyConstructible<T>::value;

    template <typename T>
    struct _isMoveConstructible
        : public _isConstructible<
              T,
              typename Std::RvalueRef<typename Std::ConstType<T>>>
    {};

    template <typename T>
    constexpr inline bool isMoveConstructible = _isMoveConstructible<T>::value;

    template <typename T, typename... Args>
    struct _isNothrowConstructible
        : Constant<bool, __is_nothrow_constructible(T, Args...)>
    {};

    template <typename T, typename... Args>
    constexpr inline bool isNothrowConstructible =
        _isNothrowConstructible<T, Args...>::value;

    template <typename T>
    struct _isNothrowCopyConstructible
        : public _isNothrowConstructible<
              T,
              typename Std::LvalueRef<typename Std::ConstType<T>>>
    {};

    template <typename T>
    constexpr inline bool isNothrowCopyConstructible =
        _isNothrowCopyConstructible<T>::value;

    template <typename T>
    struct _isNothrowMoveConstructible
        : public _isNothrowConstructible<
              T,
              typename Std::RvalueRef<typename Std::ConstType<T>>>
    {};

    template <typename T>
    constexpr inline bool isNothrowMoveConstructible =
        _isNothrowMoveConstructible<T>::value;

    template <typename T, typename U>
    struct _isAssignable : Constant<bool, __is_assignable(T, U)>
    {};

    template <typename T, typename U>
    constexpr inline bool isAssignable = _isAssignable<T, U>::value;

    template <typename T>
    struct _isDestructible : Constant<bool, __is_destructible(T)>
    {};

    template <typename T>
    constexpr inline bool isDestructible = _isDestructible<T>::value;

    // MARK: - Trivially

    template <typename T, typename... Args>
    struct _isTriviallyConstructible
        : Constant<bool, __is_trivially_constructible(T, Args...)>
    {};

    template <typename T, typename... Args>
    constexpr inline bool isTriviallyConstructible =
        _isTriviallyConstructible<T, Args...>::value;

    template <typename T>
    struct _isTriviallyCopyConstructible
        : public _isTriviallyConstructible<
              T,
              typename Std::LvalueRef<typename Std::ConstType<T>>>
    {};

    template <typename T>
    constexpr inline bool isTriviallyCopyConstructible =
        _isTriviallyCopyConstructible<T>::value;

    template <typename T>
    struct _isTriviallyMoveConstructible
        : public _isTriviallyConstructible<
              T,
              typename Std::RvalueRef<typename Std::ConstType<T>>>
    {};

    template <typename T>
    constexpr inline bool isTriviallyMoveConstructible =
        _isTriviallyMoveConstructible<T>::value;

    template <typename T, typename U>
    struct _isTriviallyAssignable
        : Constant<bool, __is_trivially_assignable(T, U)>
    {};

    template <typename T, typename U>
    constexpr inline bool isTriviallyAssignable =
        _isTriviallyAssignable<T, U>::value;

    // MARK: - Condition

    template <bool B, typename T, typename E>
    struct _Conditional
    {
        using Type = T;
    };

    template <typename T, typename E>
    struct _Conditional<false, T, E>
    {
        using Type = E;
    };

    template <bool B, typename T, typename E>
    using Conditional = typename _Conditional<B, T, E>::Type;

    // MARK: - Decay

    template <typename T>
    struct Decay
    {
        using InnerType = RemoveRef<T>;

        using Type = Conditional<
            isArray<InnerType>,
            Pointed<_makeItUnextent<InnerType>>,
            Conditional<
                isFunction<InnerType>,
                Pointed<InnerType>,
                RemoveCv<InnerType>>>;
    };
}

template <typename T>
struct Traits;

template <>
struct Traits<u8>
{
    using Type                      = u8;
    static constexpr bool isSigned  = false;
    static constexpr bool isFloat   = false;
    static constexpr bool isInteger = true;
    static constexpr u8   min       = 0;
    static constexpr u8   max       = 255;
};

template <>
struct Traits<u16>
{
    using Type                      = u16;
    static constexpr bool isSigned  = false;
    static constexpr bool isFloat   = false;
    static constexpr bool isInteger = true;
    static constexpr u16  min       = 0;
    static constexpr u16  max       = 65535;
};

template <>
struct Traits<u32>
{
    using Type                      = u32;
    static constexpr bool isSigned  = false;
    static constexpr bool isFloat   = false;
    static constexpr bool isInteger = true;
    static constexpr u32  min       = 0;
    static constexpr u32  max       = 4294967295;
};

template <>
struct Traits<u64>
{
    using Type                      = u64;
    static constexpr bool isSigned  = false;
    static constexpr bool isFloat   = false;
    static constexpr bool isInteger = true;
    static constexpr u64  min       = 0;
    static constexpr u64  max       = 18446744073709551615;
};

template <>
struct Traits<usize>
{
    using Type                       = usize;
    static constexpr bool  isSigned  = false;
    static constexpr bool  isFloat   = false;
    static constexpr bool  isInteger = true;
    static constexpr usize min       = 0;
    static constexpr usize max       = 18446744073709551615;
};

template <>
struct Traits<i8>
{
    using Type                      = i8;
    static constexpr bool isSigned  = true;
    static constexpr bool isFloat   = false;
    static constexpr bool isInteger = true;
    static constexpr i8   min       = -128;
    static constexpr i8   max       = 127;
};

template <>
struct Traits<i16>
{
    using Type                      = i16;
    static constexpr bool isSigned  = true;
    static constexpr bool isFloat   = false;
    static constexpr bool isInteger = true;
    static constexpr i16  min       = -32768;
    static constexpr i16  max       = 32767;
};

template <>
struct Traits<i32>
{
    using Type                      = i32;
    static constexpr bool isSigned  = true;
    static constexpr bool isFloat   = false;
    static constexpr bool isInteger = true;
    static constexpr i32  min       = -2147483648;
    static constexpr i32  max       = 2147483647;
};

template <>
struct Traits<i64>
{
    using Type                      = i64;
    static constexpr bool isSigned  = true;
    static constexpr bool isFloat   = false;
    static constexpr bool isInteger = true;
    static constexpr i64  min       = -9223372036854775808;
    static constexpr i64  max       = 9223372036854775807;
};

template <>
struct Traits<f32>
{
    using Type                      = f32;
    static constexpr bool isSigned  = true;
    static constexpr bool isFloat   = true;
    static constexpr bool isInteger = false;
    static constexpr f32  min       = -3.40282347e+38;
    static constexpr f32  max       = 3.40282347e+38;
};

template <>
struct Traits<f64>
{
    using Type                      = f64;
    static constexpr bool isSigned  = true;
    static constexpr bool isFloat   = true;
    static constexpr bool isInteger = false;
    static constexpr f64  min       = -1.7976931348623158e+308;
    static constexpr f64  max       = 1.7976931348623158e+308;
};

template <>
struct Traits<f128>
{
    using Type                      = f128;
    static constexpr bool isSigned  = true;
    static constexpr bool isFloat   = true;
    static constexpr bool isInteger = false;
    static constexpr f128 min = -1.18973149535723176508575932662800702e+4932;
    static constexpr f128 max = 1.18973149535723176508575932662800702e+4932;
};

template <>
struct Traits<char>
{
    always_inline static constexpr bool isDigit(char c)
    {
        return c >= '0' and c <= '9';
    }

    always_inline static constexpr bool isHexDigit(char c)
    {
        return (c >= '0' and c <= '9') or (c >= 'a' and c <= 'f') or
               (c >= 'A' and c <= 'F');
    }

    always_inline static constexpr bool isAlpha(char c)
    {
        return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z');
    }

    always_inline static constexpr bool isAlnum(char c)
    {
        return isAlpha(c) or isDigit(c);
    }

    always_inline static constexpr bool isSpace(char c)
    {
        return c == ' ' or c == '\t' or c == '\n' or c == '\r';
    }

    always_inline static constexpr bool isPrint(char c)
    {
        return c >= 0x20 and c <= 0x7E;
    }

    always_inline static constexpr bool isPunct(char c)
    {
        return (c >= 0x21 and c <= 0x2F) or (c >= 0x3A and c <= 0x40) or
               (c >= 0x5B and c <= 0x60) or (c >= 0x7B and c <= 0x7E);
    }

    always_inline static constexpr bool isLowerCase(char c)
    {
        return c >= 'a' and c <= 'z';
    }

    always_inline static constexpr bool isUpperCase(char c)
    {
        return c >= 'A' and c <= 'Z';
    }

    always_inline static constexpr char toLowerCase(char c)
    {
        return isUpperCase(c) ? c + 32 : c;
    }

    always_inline static constexpr char toUpperCase(char c)
    {
        return isLowerCase(c) ? c - 32 : c;
    }
};