#pragma once

#include <Types.h>

namespace Models::Storage::Partitions {
    struct MasterBootRecord
    {
        static constexpr u16 sc_SignatureValue = 0xAA55;

        u8  BootCode[440];
        u32 DiskSignature;
        u16 __reserved__0;
        struct PartitionEntry
        {
            u8  Status;
            u8  FirstSectorHead;
            u16 FirstSectorCylinder;
            u8  Type;
            u8  LastSectorHead;
            u16 LastSectorCylinder;
            u32 FirstSectorLBA;
            u32 SectorsCount;
        } Entries[4];
        u16 Signature;
    } __attribute__((packed));
    static_assert(
        sizeof(MasterBootRecord) == 512,
        "MasterBootRecord size must be 512 bytes");
}