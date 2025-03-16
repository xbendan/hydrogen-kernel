#pragma once

#include <Types.h>

namespace Models::Storage::Partition {
    struct MasterBootRecordProtective
    {
        u8  BootCode{ 0x00 };
        u8  ChsBegin[3]{ 0x00, 0x02, 0x00 };
        u8  Type{ 0xEE /* GPT Protective */ };
        u8  ChsEnd[3]{ 0xFF, 0xFF, 0xFF };
        u32 LBA_Begin{ 1 };
        u32 SectorsCount{ 0xFFFFFFFF };
    } __attribute__((packed));

    struct GuidPartitionTable
    {
        static constexpr u64 sc_SignatureValue = 0x5452415020494645; // EFI PART

        struct PartitionEntry
        {
            u8  TypeGuid[16];
            u8  Guid[16];
            u64 LBA_Begin;
            u64 LBA_End;
            u64 Attributes;
            u16 Name[36];
        } __attribute__((packed));
        static_assert(
            sizeof(PartitionEntry) == 128,
            "PartitionEntry size must be 128 bytes");

        u64 Signature;
        u16 Revision;
        u16 HeaderSize;
        u32 Checksum; // CRC32
        u32 __reserved__0;
        u64 LBA_Table;
        u64 LBA_Backup;
        u64 LBA_FirstUsable;
        u64 LBA_LastUsable;
        u8  DiskGuid[16];
        u64 Entries;
        u32 EntriesCount;
        u32 EntrySize;
        u32 EntriesChecksum;
    } __attribute__((packed));
}