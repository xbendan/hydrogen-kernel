#pragma once

#include <Concepts.h>
#include <Types.h>

namespace System::Collections::Generic {
    template <typename T, typename Inner = typename T::InnerType>
    concept IHasher = requires(T& t, Inner const& key) {
        { t(key) } -> Same<u32>;
    };
}