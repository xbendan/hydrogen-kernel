#pragma once

#include <TypeTraits.h>
#include <Types.h>

namespace System::Numerics {
    always_inline constexpr auto Max(auto value)
    {
        return value;
    }

    always_inline constexpr auto Max(auto first, auto... rest)
    {
        auto rhs = Max(rest...);
        return first > rhs ? first : rhs;
    }

    always_inline constexpr auto Min(auto value)
    {
        return value;
    }

    always_inline constexpr auto Min(auto first, auto... rest)
    {
        auto rhs = Min(rest...);
        return first < rhs ? first : rhs;
    }

    always_inline constexpr auto Abs(auto value)
    {
        return value < 0 ? -value : value;
    }

    always_inline constexpr auto Clamp(auto value, auto min, auto max)
    {
        return value < min ? min : value > max ? max : value;
    }

    always_inline constexpr auto Lerp(auto a, auto b, auto t)
    {
        return a + (b - a) * t;
    }

    always_inline constexpr auto
    Lerp(auto a, auto b, auto t, auto min, auto max)
    {
        return Clamp(Lerp(a, b, t), min, max);
    }

    always_inline constexpr auto
    Lerp(auto a, auto b, auto t, auto min, auto max, auto minOut, auto maxOut)
    {
        return Lerp(Lerp(a, b, t, min, max), minOut, maxOut);
    }

    always_inline constexpr auto InvLerp(auto a, auto b, auto value)
    {
        return (value - a) / (b - a);
    }

    always_inline constexpr auto Pow(auto base, auto exp)
    {
        auto result = 1;
        for (auto i = 0; i < exp; ++i) {
            result *= base;
        }
        return result;
    }

    template <typename I, typename P>
        requires Std::isIntegral<I> && Std::isFloatingPoint<P>
    always_inline constexpr I Round(P value)
    {
        if constexpr (Std::isSame<P, long double>)
            return static_cast<I>(__builtin_llrintl(value));
        if constexpr (Std::isSame<P, double>)
            return static_cast<I>(__builtin_llrint(value));
        if constexpr (Std::isSame<P, float>)
            return static_cast<I>(__builtin_llrintf(value));
    }

    namespace Math {
        constexpr f32 PI = 3.14159265358979323846f;
        constexpr f32 E  = 2.71828182845904523536f;

        always_inline f32 Sin(f32 x)
        {
            return __builtin_sinf(x);
        }

        always_inline f32 Cos(f32 x)
        {
            return __builtin_cosf(x);
        }

        always_inline f32 Tan(f32 x)
        {
            return __builtin_tanf(x);
        }

        template <typename T>
            requires Std::isArithmetic<T>
        always_inline T Sqrt(T x)
        {
            return __builtin_sqrt(x);
        }

        template <typename T>
            requires Std::isArithmetic<T>
        always_inline constexpr T FastInvSqrt(T x)
        {
            float xhalf = 0.5f * x;
            int   i     = *(int*)&x;
            i           = 0x5f3759df - (i >> 1);
            x           = *(float*)&i;
            x           = x * (1.5f - xhalf * x * x);
            return x;
        }
    }
}