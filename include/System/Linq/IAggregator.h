#pragma once

#include <Concepts.h>

namespace System::Linq {
    template <typename T, typename TResult = typename T::ResultType>
    concept IAggregator = requires(T& t) {
        typename T::ExprSet;

        { t() } -> Same<TResult>;
    };
}