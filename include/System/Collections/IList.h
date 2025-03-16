#pragma once

#include <Meta/Indexer.h>
#include <System/Collections/ICollection.h>

namespace System::Collections {
    template <typename T, typename Inner = typename T::InnerType>
    concept IList = requires(T& t, Inner const& val, usize index) {
        ICollection<T, Inner>;

        { t.operator[](index) } -> Same<Inner&>;
        { t.operator[](index) } -> Same<Inner const&>;
        { t.operator-=(Indexer{ 0 }) } -> Same<bool>;

        { t.At(index) } -> Same<Inner&>;
        { t.IndexFirst(val) } -> Same<usize>;
        { t.IndexLast(val) } -> Same<usize>;
    };
}