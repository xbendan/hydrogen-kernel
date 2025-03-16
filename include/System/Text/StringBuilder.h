#pragma once

#include <Meta/Array.h>
#include <Meta/Property.h>
#include <Meta/Tuple.h>
#include <System/Text/EncodingFormat.h>

namespace System::Text {
    class StringBuilder final
    {
    public:
        ReadonlyProperty<usize, StringBuilder> Capacity;

        template <typename T>
        StringBuilder& Append(T const& value);

        template <typename T>
        StringBuilder& operator<<(T const& value);

        template <typename... Ts>
        StringBuilder& operator<<(Tuple<Ts...> const& tuple);

    private:
        Array<Rune> m_runes;
    };
}