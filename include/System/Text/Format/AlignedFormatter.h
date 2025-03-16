#pragma once

#include <Types.h>

namespace System::Text::Format {
    enum struct Align
    {
        LEFT,
        RIGHT,
        CENTER
    };

    template <typename T>
    struct Aligned
    {
        T     _inner;
        Align _align;
        usize _width;
    };
}