#pragma once

#include <Meta/Array.h>
#include <Meta/Optional.h>
#include <System/Collections/LinkedList.h>
#include <System/IO/Device.h>
#include <System/IO/Storage/StorageDevice.h>

namespace System::IO::Storage {
    using Collections::LinkedList;

    class StorageControllerDevice : public Device
    {
    public:
        virtual Array<StorageDevice*>    GetAll()             = 0;
        virtual Optional<StorageDevice*> GetByIndex(u8 index) = 0;
    };
}