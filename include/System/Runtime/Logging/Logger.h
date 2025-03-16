#pragma once

#include <System/IO/TextWriter.h>
#include <System/Runtime/Logging/Level.h>
#include <System/Text/Encoding.UTF8.h>

namespace System::Runtime::Logging {
    using IO::TextWriterBase;
    using Text::UTF8;

    class Logger : public TextWriterBase<UTF8>
    {
    public:
    private:
        Level m_minLevel;
    };
}