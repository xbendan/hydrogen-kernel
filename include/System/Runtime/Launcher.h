#pragma once

#include <System/IO/Device.h>
#include <System/Memory/MemoryMap.h>
#include <System/Runtime/Intrinsics/Types.h>

#include <System/Text/StringView.h>

namespace System::Runtime {
    using IO::Device;
    using MemoryService::MemoryMap;
    using MemoryService::MemoryMapEntry;

    class Launcher
    {
    public:
        Launcher& AddMemmapEntry(MemoryMapEntry entry)
        {
            m_memmap[m_memmapSize++] = entry;

            return *this;
        }

        template <Intrinsics::IntrinsicsType arch>
        [[noreturn]] void Startup();

    private:
        u64     m_bootTime;
        strview m_bootloaderName;

        MemoryMap m_memmap;
        usize     m_memmapSize;
    };
}