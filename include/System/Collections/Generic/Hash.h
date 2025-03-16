#pragma once

#include <System/Collections/Generic/IHash.h>
#include <System/Security/Cryptography/Hash.h>
#include <System/Text/String.h>
#include <System/Text/StringView.h>
#include <TypeTraits.h>
#include <Types.h>

namespace System::Collections::Generic {
    template <typename T>
    struct Hash;

    template <typename T>
        requires Std::isIntegral<T> or Std::isPointer<T>
    struct Hash<T>
    {
        using InnerType = T;

        always_inline constexpr u32 operator()(T const& value)
        {
            return static_cast<u32>(value) * 2654435761u;
        }
    };

    template <>
    struct Hash<char const*>
    {
        using InnerType = char const*;

        always_inline constexpr u32 operator()(char const* value)
        {
            u32 hash = 0;
            while (*value) {
                hash = *value + (hash << 6) + (hash << 16) - hash;
                value++;
            }
            return hash;
        }
    };

    template <>
    struct Hash<strview>
    {
        using InnerType = strview;

        always_inline constexpr u32 operator()(strview const& value)
        {
            return Crypto::MurmurHash3x86(value._buf, value._len);
        }
    };

    template <>
    struct Hash<string>
    {
        using InnerType = string;

        always_inline constexpr u32 operator()(string const& value)
        {
            return Crypto::MurmurHash3x86(value._data, value._len);
        }
    };
}