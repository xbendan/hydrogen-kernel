#include <System/Runtime/Launcher.h>

namespace System::Runtime {
    template <>
    [[noreturn]] void Launcher::Startup<X86_64>()
    {
    }
}