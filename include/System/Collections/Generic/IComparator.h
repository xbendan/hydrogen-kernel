#pragma once

#include <Concepts.h>
#include <Types.h>

namespace System::Collections::Generic {
    template <typename T>
    concept IComparator = requires(T& t, T const& a, T const& b) {
        { t.Equals(a, b) } -> Same<bool>;
    };
}