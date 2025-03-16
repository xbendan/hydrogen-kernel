#pragma once

#include <Types.h>

namespace System {
    enum struct Whence
    {
        BEGIN,
        CURRENT,
        END,
    };

    struct SeekPos
    {
        usize  offset;
        Whence whence;

        static constexpr SeekPos Begin(usize offset = 0)
        {
            return { offset, Whence::BEGIN };
        }

        static constexpr SeekPos Current(usize offset = 0)
        {
            return { offset, Whence::CURRENT };
        }

        static constexpr SeekPos End(usize offset = 0)
        {
            return { offset, Whence::END };
        }
    };
}