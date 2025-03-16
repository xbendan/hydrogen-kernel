#pragma once

#include <Meta/Property.h>
#include <Meta/Result.h>
#include <System/Text/StringView.h>
#include <System/Uuid.h>

namespace System::IO {
    using Text::StringView;

    class Device
    {
    public:
        enum class Type : u64
        {
            Biometric            = (1 << 0),
            Bluetooth            = (1 << 1),
            Camera               = (1 << 2),
            DisplayAdapter       = (1 << 3),
            DisplayOutput        = (1 << 4),
            Firmware             = (1 << 5),
            HumanInterface       = (1 << 6),
            Keyboard             = (1 << 7),
            Multimedia           = (1 << 8),
            NetworkAdapter       = (1 << 9),
            Portable             = (1 << 10),
            PointerDevice        = (1 << 11),
            Printer              = (1 << 12),
            PowerSupply          = (1 << 13),
            Processor            = (1 << 14),
            Security             = (1 << 15),
            Sensor               = (1 << 16),
            StorageDevice        = (1 << 17),
            StorageController    = (1 << 18),
            SoftwareDevice       = (1 << 19),
            SystemDevice         = (1 << 20),
            TimeDevice           = (1 << 21),
            PeripheralDevice     = (1 << 22),
            PeripheralController = (1 << 23)
        };

        Device(strview name, Type deviceType);

        virtual ~Device() = default;

        /**
         * @brief called in the early stage of kernel bootup
         *
         * This method is called in the early stage, when most features of the
         * kernel are not yet available, except for the basic memory management,
         * the console, and the logging system.
         *
         * @return Result<>
         */
        virtual Result<> OnInitialize() { return Ok(); }

        /**
         * @brief called when most features of the kernel are available
         *
         * This method is called when most features, such as the memory manager,
         * the scheduler, and the filesystem, are available. Before entering the
         * main loop of the kernel, this method is called to complete the
         * initialization of the device. (Before entering user mode)
         *
         * @return Result<>
         */
        virtual Result<> OnStartup() { return Ok(); }

        /**
         * @brief called when the system is shutting down
         *
         * @return Result<>
         */
        virtual Result<> OnShutdown() { return Ok(); }

        Property<strview>              Name;
        Property<Type>                 Type;
        ReadonlyProperty<Uuid, Device> UniqueId;
    };
}