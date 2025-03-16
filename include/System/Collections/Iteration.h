#pragma once

#include <Concepts.h>
#include <TypeTraits.h>

namespace System::Collections {
    template <typename T, typename Inner = T::InnerType>
    concept Iterator = requires(T t) {
        // Equatable<TInnerType>;
        { *t } -> Same<Inner&>;
        { ++t } -> Same<bool>;
        { t != Empty{} } -> Same<bool>;
    };

    template <typename T>
    concept Iterable = requires(T t) {
        { t.begin() };
        { t.end() };
    };
}