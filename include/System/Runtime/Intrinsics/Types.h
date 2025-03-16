#pragma once

#include <Types.h>

namespace System::Runtime::Intrinsics {
    enum IntrinsicsType : u8
    {
        X86_64,
        LoongArch64,
        Aarch64,
    };
}

using System::Runtime::Intrinsics::IntrinsicsType::Aarch64;
using System::Runtime::Intrinsics::IntrinsicsType::LoongArch64;
using System::Runtime::Intrinsics::IntrinsicsType::X86_64;