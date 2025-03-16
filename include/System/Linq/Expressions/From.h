#pragma once

#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerable.h>
#include <System/Linq/IEnumerator.h>

#include <Meta/Array.h>

namespace System::Linq::Expressions {
    template <typename TSource>
    constexpr Enumerator<TSource, ArrayEnumerator<TSource>> From(
        TSource const* source,
        usize          len)
    {
        return { ArrayEnumerator(source, len) };
    }

    template <typename TSource>
    constexpr Enumerator<TSource, ArrayEnumerator<TSource>> From(
        Array<TSource>& source)
    {
        return { ArrayEnumerator(*source, source.Length()) };
    }

    template <typename TSource, usize TLen>
    constexpr Enumerator<TSource, ArrayEnumerator<TSource>> From(
        Array<TSource, TLen>& source)
    {
        using T = decltype(*source);
        return { ArrayEnumerator(*source, source.Length()) };
    }

    template <
        IEnumerable TEnumerable,
        typename Inner = typename TEnumerable::Inner>
    constexpr Enumerator<Inner, TEnumerable> From(TEnumerable const& source)
    {
        return Enumerator{ source.GetEnumerator() };
    }
}