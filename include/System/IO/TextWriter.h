#pragma once

#include <Concepts.h>
#include <System/Text/Encoding.UTF8.h>
#include <System/Text/EncodingFormat.h>
#include <System/Text/StringView.h>

namespace System::IO {
    using Text::EncodingFormat;
    using Text::StringView;
    using Text::UTF8;

    template <typename T, typename E = UTF8, typename U = E::Unit>
    concept ITextWriter = requires(T& t, U u, StringView<E> s) {
        EncodingFormat<E>;
        { t(u) } -> Same<void>;
        { t.Write(u) } -> Same<void>;
        { t.Write(s) } -> Same<usize>;
        { t.Flush() } -> Same<usize>;
    };

    template <EncodingFormat E = UTF8, typename U = E::Unit>
    class TextWriterBase
    {
    public:
        using Encoding = E;
        using Unit     = U;

        virtual void  operator()(Unit u)     = 0;
        virtual void  Write(Unit u)          = 0;
        virtual usize Write(StringView<E> s) = 0;
        virtual usize Flush()                = 0;
    };
}