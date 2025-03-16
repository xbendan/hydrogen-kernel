#include <System/IO/TextWriter.h>
#include <System/Text/EncodingFormat.h>
#include <System/Text/Format/Formatter.h>

namespace System::Text::Format {
    using IO::ITextWriter;

    template <>
    struct Formatter<bool>
    {
        usize Format(ITextWriter auto& writer, bool value)
        {
            // return writer.Write(value ? "true" : "false");
        }
    };
}