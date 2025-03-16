#pragma once

#include <System/IO/Device.h>
#include <System/IO/Storage/StorageAbility.h>
#include <System/Stream.h>

namespace System::IO::Storage {
    class StorageDevice
        : public Device
        , public StorageDuplexable
    {
    public:
    };
}