#pragma once

#include <Meta/Slice.h>
#include <System/IO/Stream.h>

namespace System::IO::Storage {
    struct StorageReadable
    {
        virtual usize Read(u64, Bytes)     = 0;
        virtual usize Read(SeekPos, Bytes) = 0;
    };

    struct StorageWritable
    {
        virtual usize Write(u64, Bytes)     = 0;
        virtual usize Write(SeekPos, Bytes) = 0;
    };

    struct StorageDuplexable
        : StorageReadable
        , StorageWritable
    {};
}