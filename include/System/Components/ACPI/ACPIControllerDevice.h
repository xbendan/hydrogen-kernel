#pragma once

#include <Definitions/Generic/ACPI.h>
#include <System/IO/Device.h>
#include <System/Text/StringView.h>

namespace System::Components {
    using namespace ACPI;
    using IO::Device;

    class ACPIControllerDevice : public Device
    {
        template <ChildTypeOf<TableDataGeneric> TTable>
        TTable* LookupTable(strview name) const;

        Result<> OnInitialize() override;
    };
}