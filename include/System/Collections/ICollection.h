#pragma once

#include <Concepts.h>
#include <Types.h>

namespace System::Collections {
    template <typename T, typename Inner = typename T::InnerType>
    concept ICollection = requires(T& t, Inner const& val, Empty empty) {
        { t.Count } -> Same<usize>;

        { t.operator bool() };
        { t.operator==(empty) } -> Same<bool>;
        { t.operator!=(empty) } -> Same<bool>;
        { t.operator=(empty) };
        { t.operator+=(val) };
        { t.operator-=(val) };

        { t.Contains(val) } -> Same<bool>;
    };
}