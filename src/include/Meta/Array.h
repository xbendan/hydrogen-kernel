#pragma once

#include <StdC++/Types.h>

namespace Meta {
    template <typename, usize>
    struct Array;

    template <typename T, usize TCount>
    struct Array
    {};
}

using Meta::Array;