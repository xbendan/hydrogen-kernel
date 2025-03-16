#pragma once

#include <Concepts.h>
#include <Meta/Cursor.h>
#include <System/Text/Rune.h>
#include <Types.h>

namespace System::Text {
    template <typename T>
    concept EncodingFormat = requires(
        T                         t,
        Rune&                     r,
        typename T::Unit          unit,
        Cursor<typename T::Unit>& c) {
        { T::GetUnitLength(unit) } -> Same<u8>;
        { T::GetRuneLength(r) } -> Same<u8>;
        { T::DecodeUnit(r, c) } -> Same<bool>;
        { T::EncodeUnit(Rune{}, c) } -> Same<bool>;
    };

    template <typename T, typename U>
    concept EncodeOutput = requires(T t, U u) {
        { t.Append(u) };
    };

    template <typename T, typename U>
    concept DecodeInput = requires(T t, U u) {
        { t.Next() } -> ConvertibleTo<U>;
        { t.Remain() } -> ConvertibleTo<usize>;
    };
}