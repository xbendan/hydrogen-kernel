#include <System/Memory/PageFrame.h>

namespace System::MemoryService {
    constexpr void PageFrame::operator=(
        Tuple<Flags<Hal::PmmFlags>, u8, uptr> data)
    {
        for (int i = 0; i < (1 << data.v1); i++) {
            this[i] = { ._flags   = data.v0,
                        ._level   = 1,
                        ._address = data.v2 + i * PAGE_SIZE_4K };
        }
        MakeRegionAt(data.v2, (1 << data.v1) * PAGE_SIZE_4K).Zeroize();

        _level       = data.v1;
        _chainLength = (1 << data.v1);
    }
}