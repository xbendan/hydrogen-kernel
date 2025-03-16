#pragma once

#include <System/Linq/IEnumerator.h>

namespace System::Linq {
    template <typename T, typename Inner = T::InnerType>
    concept IEnumerable = requires(T& t) {
        { t.GetEnumerator() } -> IEnumerator<Inner>;
        // { t.begin() } -> IEnumerator;
        // { t.end() } -> Same<Empty>;
    };
}