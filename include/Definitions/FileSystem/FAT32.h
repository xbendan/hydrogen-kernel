#pragma once

#include <Types.h>

namespace FAT32 {
    struct ParamBlock
    {
        u16 BytesPerSector;
        u8  SectorsPerCluster;
        u16 SectorsReserved;
        u8  NumberOfFATs;
        u16 NumberOfRootEntries;
        u16 TotalSectors16;
        u8  Media;
        u16 SectorsPerFAT16;
        u16 SectorsPerTrack;
        u16 HeadCount;
        u32 HiddenSectors;
        u32 TotalSectors32;
        u32 SectorsPerFAT32;

        union
        {
            u16 ExtFlags;
            struct
            {
                u16 ActiveFAT : 4;
                u16 __reserved__0 : 3;
                u16 MirroredFAT : 1;
                u16 __reserved__1 : 8;
            };
        };

        u16 Version;
        u32 RootCluster;
        u16 FsInfo;
        u16 BackupBootSector;
    };
}