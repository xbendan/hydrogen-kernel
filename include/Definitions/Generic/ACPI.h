#pragma once

#include <Types.h>

namespace ACPI {
    struct RootSystemDescPointer
    {
        char Signature[8];
        u8   Checksum;
        char OemID[6];
        u8   Revision;
        u32  RsdtAddress;
    } __attribute__((packed));

    struct ExtendedSystemDescPointer : public RootSystemDescPointer
    {
        u32 Length;
        u64 XsdtAddress;
        u8  ChecksumEx;
        u8  __reserved__0[3];
    } __attribute__((packed));

    struct TableDataGeneric
    {
        char Signature[4];
        u32  Length;
        u8   Revision;
        u8   Checksum;
        char OemID[6];
        char OemTableID[8];
        u32  OemRevision;
        u32  CreatorId;
        u32  CreatorRevision;
    } __attribute__((packed));

    struct RootSystemDescTable : public TableDataGeneric
    {
        u32 Pointers[];
    } __attribute__((packed));

    struct ExtendedSystemDescTable : public TableDataGeneric
    {
        u64 Pointers[];
    } __attribute__((packed));

    struct AddressPack
    {
        /**
         * @brief address space indicates where to read data
         * 0: System Memory
         * 1: System I/O
         * 2: PCI Configuration Space
         * 3: Embedded Controller
         * 4: @deprecated System Management Bus
         * 5: System CMOS
         * 6: PCI Device BAR Target
         * 7: @deprecated Intelligent Platform Management Infrastructure
         * 8: @deprecated General Purpose I/O
         * 9: @deprecated Generic Serial Bus
         * 0x0a: @deprecated Platform Communication Channel
         * 0x0b-0x7f: reserved
         * 0x80-0xff: OEM defined
         */
        u8  AddressSpace;
        u8  BitWidth;
        u8  BitOffset;
        u8  AccessSize;
        u64 Address;
    } __attribute__((packed));

    struct MultiApicDescTable /* Multiple APIC Description Table */
        : public TableDataGeneric
    {
        u32 Address;
        u32 Flags;

        struct Entry
        {
            u8 Type;
            u8 Length;
        } __attribute__((packed)) _entries[];

        struct LocalApic : public Entry
        {
            u8  ProcessorId;
            u8  ApicId;
            u32 Flags;
        } __attribute__((packed));

        struct IOApic : public Entry
        {
            u8  ApicId;
            u8  __reserved__0;
            u32 Address;
            u32 gSiB;
        } __attribute__((packed));

        struct InterruptServiceOverride : public Entry
        {
            u8  BusSource;
            u8  IrqSource;
            u32 gSi;
            u16 Flags;
        } __attribute__((packed));

        struct NonMaskableInterrupt : public Entry
        {
            u8  ProcessorId;
            u16 Flags;
            u8  lInt;
        } __attribute__((packed));

        struct Localx2Apic : public Entry
        {
            u16 __reserved__0;
            u32 x2apicId;
            u32 Flags;
            u32 Uid;
        } __attribute__((packed));

        struct Nmix2Apic : public Entry
        {
            u16 Flags;
            u32 Uid;
            u8  lInt;
            u8  __reserved__0[3];
        } __attribute__((packed));
    } __attribute__((packed));

    using MADT = MultiApicDescTable;

    struct HighPrecisionEventTable : public TableDataGeneric
    {
        u8          HwRevId;
        u8          Info;
        u16         PCIVendorId;
        AddressPack Address;
        u8          NumberHPET;
        u16         MinTick;
        u8          PageProtection;
    } __attribute__((packed));

    using HPET = HighPrecisionEventTable;

    struct PCIExpressSpecTable : public TableDataGeneric
    {
        u64 __reserved__0;

        struct AddressPacket
        {
            u64 Base;
            u16 Sgn;
            u8  BusStart;
            u8  BusEnd;
            u32 __reserved__0;
        } __attribute__((packed)) BaseAddresses[];
    } __attribute__((packed));

    struct FixedAcpiDescTable /* Fixed ACPI Description Table */
        : public TableDataGeneric
    {
        u32 FwCtrl;
        u32 DSDT;

        u8 __reserved__0;

        u8  PreferredPowerManagementProfile;
        u16 SCIInterrupt;
        u32 SMICommandPort;
        u8  Enable;
        u8  Disable;
        u8  S4BIOSReq;
        u8  PStateControl;
        u32 PM1aEventBlock;
        u32 PM1bEventBlock;
        u32 PM1aCtrlBlock;
        u32 PM1bCtrlBlock;
        u32 PM2CtrlBlock;
        u32 PMTTimerBlock;
        u32 GPE0Block;
        u32 GPE1Block;
        u8  PM1EventLength;
        u8  PM1CtrlLength;
        u8  PM2CtrlLength;
        u8  PMTTimerLength;
        u8  GPE0Length;
        u8  GPE1Length;
        u8  GPE1Base;
        u8  CStateCtrl;
        u16 WorstC2Latency;
        u16 WorstC3Latency;
        u16 FlushSize;
        u16 FlushStride;
        u8  DutyOffset;
        u8  DutyWidth;
        u8  DayAlarm;
        u8  MonthAlarm;
        u8  Century;

        u16 BootFlagIAPC;

        u8 __reserved__1;

//         struct
//         {
//             /*
//                 If set, signifies that the WBINVD instruction
//                 correctly flushes
// the processor caches, maintains memory coherency, and upon completion
// of the instruction, all caches for the current processor contain no
// cached data other than what OSPM references and allows to be cached.
// If this flag is not set, the ACPI OS is responsible for disabling all
// ACPI features that need this function. This field is maintained for
// ACPI 1.0 processor compatibility on existing systems. Processors in
// new ACPIcompatible systems are required to support this function and
// indicate this to OSPM by setting this field.
//                 如果设置，则表示WBINVD指令正确地刷新处理器缓存，保持内存一致性，并且在指令完成后，当前处理器的所有缓存都不包含除了OSPM引用和允许缓存的数据之外的缓存数据。
// 如果不设置该标志，则ACPI操作系统负责禁用所有需要该功能的ACPI特性。
// 该字段是为了在现有系统上兼容ACPI 1.0处理器而维护的。
// 新的acpiccompatible系统中的处理器需要支持此功能，并通过设置此字段向OSPM表明此功能。
//             */
//             u32 WbInvd : 1;
//             u32 WbInvdFlush : 1;
//             u32 ProcC1 : 1;
//             u32 P_LVL2Up : 1;
//         } _flags;
#define FADT_FLAGS_WBINVD (1 << 0)
#define FADT_FLAGS_WBINVD_FLUSH (1 << 1)
#define FADT_FLAGS_PROC_C1 (1 << 2)
#define FADT_FLAGS_P_LVL2UP (1 << 3)
#define FADT_FLAGS_PWR_BUTTON (1 << 4)
#define FADT_FLAGS_SLP_BUTTON (1 << 5)
#define FADT_FLAGS_FIX_RTC (1 << 6)
#define FADT_FLAGS_RTC_S4 (1 << 7)
#define FADT_FLAGS_TMR_VAL_EXT (1 << 8)
#define FADT_FLAGS_DCK_CAP (1 << 9)
#define FADT_FLAGS_RESET_REG_SUP (1 << 10)
#define FADT_FLAGS_SEALED_CASE (1 << 11)
#define FADT_FLAGS_HEADLESS (1 << 12)
#define FADT_FLAGS_CPU_SW_SLP (1 << 13)
#define FADT_FLAGS_PCI_EXP_WAK (1 << 14)
#define FADT_FLAGS_USE_PLATFORM_CLOCK (1 << 15)
#define FADT_FLAGS_S4_RTC_STS_VALID (1 << 16)
#define FADT_FLAGS_REMOTE_POWER_ON_CAP (1 << 17)
#define FADT_FLAGS_APIC_CLUSTER (1 << 18)
#define FADT_FLAGS_APIC_PHYSICAL (1 << 19)
#define FADT_FLAGS_HW_REDUCED_ACPI (1 << 20)
#define FADT_FLAGS_LOW_POWER_S0 (1 << 21)
        u32 _flags;

        AddressPack ResetRegistry;
        u8          ResetValue;
        u16         BootFlagARM;
        u8          Minor;

        u64         x_FwCtrl;
        u64         x_DSDT;
        AddressPack x_PM1aEventBlock;
        AddressPack x_PM1bEventBlock;
        AddressPack x_PM1aCtrlBlock;
        AddressPack x_PM1bCtrlBlock;
        AddressPack x_PM2CtrlBlock;
        AddressPack x_PMTTimerBlock;
        AddressPack x_GPE0Block;
        AddressPack x_GPE1Block;
        AddressPack x_SleepCtrl;
        AddressPack x_SleepStatus;
        u64         x_HypervisorVendorId;
    } __attribute__((packed));

    struct FACS /* Firmware ACPI Control Structure */
    {
    } __attribute__((packed));

    struct SmartBatteryDescTable /* Smart Battery Description Table */
        : public TableDataGeneric
    {
        u32 WarningEnergyLevel;
        u32 LowEnergyLevel;
        u32 CriticalEnergyLevel;
    } __attribute__((packed));

    struct EmbedCtrlBootResTable /* Embedded Controller Boot Resources Table */
        : public TableDataGeneric
    {
        AddressPack ECControl;
        AddressPack ECData;
        u32         UID;
        u8          GPEBit;
        u8          ECId[];
    } __attribute__((packed));

    struct DiffSystemDescTable /* Differentiated System Description Table */
        : public TableDataGeneric
    {
        u8 AML[];
    };

    struct SecdSystemDescTable /* Secondary System Description Table */
        : public TableDataGeneric
    {
        u8 AML[];
    };
}