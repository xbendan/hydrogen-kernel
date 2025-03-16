#pragma once

#include <System/Collections/LinkedList.h>
#include <System/IO/TextWriter.h>
#include <System/Runtime/Logging/Level.h>
#include <System/Runtime/Logging/Logger.h>
#include <System/Text/Encoding.UTF8.h>
#include <System/Text/EncodingFormat.h>

namespace System::Runtime::Logging {
    using Collections::LinkedList;
    using IO::TextWriterBase;
    using Text::EncodingFormat;
    using Text::UTF8;

    class LoggerFactory
    {
    public:
        LoggerFactory& WithMinimumLevel(Level level);
        LoggerFactory& UseName(strview name);
        LoggerFactory& UseTemplate(strview format);
        LoggerFactory& UseMessageRecords(bool use);
        LoggerFactory& Write(TextWriterBase<UTF8>* writer);

    private:
        strview m_format;
        Level   m_minLevel;

        LinkedList<TextWriterBase<UTF8>*> m_writers;
    };
}