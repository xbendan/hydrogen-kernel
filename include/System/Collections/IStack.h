#pragma once

#include <System/Collections/ICollection.h>

namespace System::Collections {
    template <typename T, typename Inner = typename T::InnerType>
    concept IStack = requires(T& t, Inner const& val) {
        ICollection<T, Inner>;

        { t() } -> Same<Inner&>;
        { t() } -> Same<Inner const&>;
        { t.operator<<(val) };
        { t.operator>>(val) } -> Same<bool>;
    };
}