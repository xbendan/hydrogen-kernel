#pragma once

#include <Types.h>

namespace TarFS {
    static constexpr usize sc_FileNameLength = 100;

    enum FileType : u8
    {
        Regular              = '0',
        HardLink             = '1',
        SymbolicLink         = '2',
        CharacterDevice      = '3',
        BlockDevice          = '4',
        Directory            = '5',
        NamedPipeline        = '6',
        Contiguous           = '7',
        GlobalExtendedHeader = 'g',
        ExtendedHeader       = 'x'
    };

    struct FileInfo
    {
        char     Name[sc_FileNameLength];
        u64      Mode;
        u64      OwnerID;
        u64      GroupID;
        byte     Size[12];
        byte     LastModified[12];
        byte     Checksum[8];
        FileType Type;
        char     LinkName[sc_FileNameLength];
        u32      UStarIndicator;
        char     UStarVersion[2]{ '0', '0' };
        char     OwnerName[32];
        char     GroupName[32];
        u64      DeviceMajor;
        u64      DeviceMinor;
        char     Prefix[155];
    };
    static_assert(sizeof(FileInfo) == 512, "FileInfo size must be 512 bytes");
}