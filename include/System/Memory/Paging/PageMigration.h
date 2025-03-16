#include <Types.h>

namespace System::MemoryService {
    enum class Migration : u8
    {
        UNMOVABLE,
        MOVABLE,
        RECLAIMABLE,
        ISOLATED,

        Count
    };
}