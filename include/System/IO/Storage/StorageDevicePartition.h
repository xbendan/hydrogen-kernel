#pragma once

#include <System/IO/Storage/StorageAbility.h>

namespace System::IO::Storage {
    class StorageDevice;

    class StorageDevicePartition final : public StorageReadable
    {
    public:
        usize Read(u64, Bytes) override;
        usize Read(SeekPos, Bytes) override;

    private:
        StorageDevice* m_device;
    };
}