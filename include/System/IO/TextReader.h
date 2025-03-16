#pragma once

#include <Concepts.h>
#include <System/Text/Encoding.UTF8.h>
#include <System/Text/EncodingFormat.h>
#include <System/Text/StringBuffer.h>

namespace System::IO {
    using Text::EncodingFormat;
    using Text::Rune;
    using Text::UTF8;

    template <typename T, typename E = UTF8, typename U = E::Unit>
    concept ITextReader = requires(T& t, U u) {
        EncodingFormat<E>;
        { t() } -> Same<U>;
        { t.Read(u) } -> Same<Rune>;
        { t.Flush() } -> Same<usize>;
    };

    template <EncodingFormat E = UTF8, typename U = E::Unit>
    class TextReaderBase
    {
    public:
        using Encoding = E;
        using Unit     = U;

        virtual U     operator()() = 0;
        virtual Rune  Read(Unit u) = 0;
        virtual usize Flush()      = 0;
    };
}