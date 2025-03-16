#pragma once

#include <Concepts.h>

namespace System::Linq {
    template <typename T, typename Inner = T::InnerType>
    concept IEnumerator = requires(T t) {
        {
            *t
        } -> Either<Std::RemoveRef<Inner>, Std::LvalueRef<Inner>>; // Current
        { ++t } -> Same<T&>;                                       // MoveNext
        { t != Empty{} } -> Same<bool>;                            // HasNext
        { t = Empty{} } -> Same<void>;                             // Reset
    };
}