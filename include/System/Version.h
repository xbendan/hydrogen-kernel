#pragma once

#include <System/Text/StringView.h>
#include <Types.h>

namespace System {
    struct Version
    {
        u8      Major;
        u8      Minor;
        u16     Build;
        strview PatchID;
    };

    always_inline bool operator<(Version const& lhs, Version const& rhs)
    {
        if (lhs.Major < rhs.Major)
            return true;
        if (lhs.Major > rhs.Major)
            return false;

        if (lhs.Minor < rhs.Minor)
            return true;
        if (lhs.Minor > rhs.Minor)
            return false;

        if (lhs.Build < rhs.Build)
            return true;
        if (lhs.Build > rhs.Build)
            return false;

        return false;
    }

    always_inline bool operator>(Version const& lhs, Version const& rhs)
    {
        if (lhs.Major > rhs.Major)
            return true;
        if (lhs.Major < rhs.Major)
            return false;

        if (lhs.Minor > rhs.Minor)
            return true;
        if (lhs.Minor < rhs.Minor)
            return false;

        if (lhs.Build > rhs.Build)
            return true;
        if (lhs.Build < rhs.Build)
            return false;

        return false;
    }
}