#define CPUID_ECX_SSE3 (1 << 0)
#define CPUID_ECX_PCLMUL (1 << 1)
#define CPUID_ECX_DTES64 (1 << 2)
#define CPUID_ECX_MONITOR (1 << 3)
#define CPUID_ECX_DS_CPL (1 << 4)
#define CPUID_ECX_VMX (1 << 5)
#define CPUID_ECX_SMX (1 << 6)
#define CPUID_ECX_EST (1 << 7)
#define CPUID_ECX_TM2 (1 << 8)
#define CPUID_ECX_SSSE3 (1 << 9)
#define CPUID_ECX_CID (1 << 10)
#define CPUID_ECX_FMA (1 << 12)
#define CPUID_ECX_CX16 (1 << 13)
#define CPUID_ECX_ETPRD (1 << 14)
#define CPUID_ECX_PDCM (1 << 15)
#define CPUID_ECX_PCIDE (1 << 17)
#define CPUID_ECX_DCA (1 << 18)
#define CPUID_ECX_SSE4_1 (1 << 19)
#define CPUID_ECX_SSE4_2 (1 << 20)
#define CPUID_ECX_x2APIC (1 << 21)
#define CPUID_ECX_MOVBE (1 << 22)
#define CPUID_ECX_POPCNT (1 << 23)
#define CPUID_ECX_AES (1 << 25)
#define CPUID_ECX_XSAVE (1 << 26)
#define CPUID_ECX_OSXSAVE (1 << 27)
#define CPUID_ECX_AVX (1 << 28)

#define CPUID_EDX_FPU (1 << 0)
#define CPUID_EDX_VME (1 << 1)
#define CPUID_EDX_DE (1 << 2)
#define CPUID_EDX_PSE (1 << 3)
#define CPUID_EDX_TSC (1 << 4)
#define CPUID_EDX_MSR (1 << 5)
#define CPUID_EDX_PAE (1 << 6)
#define CPUID_EDX_MCE (1 << 7)
#define CPUID_EDX_CX8 (1 << 8)
#define CPUID_EDX_APIC (1 << 9)
#define CPUID_EDX_SEP (1 << 11)
#define CPUID_EDX_MTRR (1 << 12)
#define CPUID_EDX_PGE (1 << 13)
#define CPUID_EDX_MCA (1 << 14)
#define CPUID_EDX_CMOV (1 << 15)
#define CPUID_EDX_PAT (1 << 16)
#define CPUID_EDX_PSE36 (1 << 17)
#define CPUID_EDX_PSN (1 << 18)
#define CPUID_EDX_CLF (1 << 19)
#define CPUID_EDX_DTES (1 << 21)
#define CPUID_EDX_ACPI (1 << 22)
#define CPUID_EDX_MMX (1 << 23)
#define CPUID_EDX_FXSR (1 << 24)
#define CPUID_EDX_SSE (1 << 25)
#define CPUID_EDX_SSE2 (1 << 26)
#define CPUID_EDX_SS (1 << 27)
#define CPUID_EDX_HTT (1 << 28)
#define CPUID_EDX_TM1 (1 << 29)
#define CPUID_EDX_IA64 (1 << 30)
#define CPUID_EDX_PBE (1 << 31)

// Vendor strings from CPUs.
#define CPUID_VENDOR_AMD 0x68747541 // "AuthenticAMD"
#define CPUID_VENDOR_AMD_OLD                                                   \
    0x69444D41 // "AMDisbetter!" // Early engineering samples of AMD K5
               // processor
#define CPUID_VENDOR_INTEL 0x756e6547         // "GenuineIntel"
#define CPUID_VENDOR_VIA 0x20414956           // "VIA VIA VIA "
#define CPUID_VENDOR_TRANSMETA 0x756e6547     // "GenuineTMx86"
#define CPUID_VENDOR_TRANSMETA_OLD 0x6d726f54 // "TransmetaCPU"
#define CPUID_VENDOR_CYRIX 0x69727943         // "CyrixInstead"
#define CPUID_VENDOR_CENTAUR 0x746e6543       // "CentaurHauls"
#define CPUID_VENDOR_NEXGEN 0x6e6f6547        // "NexGenDriven"
#define CPUID_VENDOR_UMC 0x20434d55           // "UMC UMC UMC "
#define CPUID_VENDOR_SIS 0x49505353           // "SiS SiS SiS "
#define CPUID_VENDOR_NSC 0x736c7561           // "Geode by NSC"
#define CPUID_VENDOR_RISE 0x65736952          // "RiseRiseRise"
#define CPUID_VENDOR_VORTEX 0x74726f56        // "Vortex86 SoC"
#define CPUID_VENDOR_AO486 0x6f74694d         // "MiSTer AO486"
#define CPUID_VENDOR_AO486_OLD 0x6e756965     // "GenuineAO486"
#define CPUID_VENDOR_ZHAOXIN 0x6e616853       // "  Shanghai  "
#define CPUID_VENDOR_HYGON 0x6e6f6779         // "HygonGenuine"
#define CPUID_VENDOR_ELBRUS 0x20444b45        // "E2K MACHINE "

// Vendor strings from hypervisors.
#define CPUID_VENDOR_QEMU 0x47435447          // "TCGTCGTCGTCG"
#define CPUID_VENDOR_KVM 0x4d564b20           // " KVMKVMKVM  "
#define CPUID_VENDOR_VMWARE 0x61774d56        // "VMwareVMware"
#define CPUID_VENDOR_VIRTUALBOX 0x6f784256    // "VBoxVBoxVBox"
#define CPUID_VENDOR_XEN 0x4d4d564e           // "XenVMMXenVMM"
#define CPUID_VENDOR_HYPERV 0x726f6369        // "Microsoft Hv"
#define CPUID_VENDOR_PARALLELS 0x70726c20     // " prl hyperv "
#define CPUID_VENDOR_PARALLELS_ALT 0x7079726c // " lrpepyh vr "
// Sometimes Parallels incorrectly encodes "prlhyperv" as "lrpepyh vr" due to an
// endianness mismatch.
#define CPUID_VENDOR_BHYVE 0x65766268 // "bhyve bhyve "
#define CPUID_VENDOR_QNX 0x53514d56   // " QNXQVMBSQG "

#define CPUID_TYPE_VENDOR 0U
#define CPUID_TYPE_FEATURES 1U
#define CPUID_TYPE_CACHE 2U
#define CPUID_TYPE_SERIAL 3U
#define CPUID_TYPE_MONITOR 5U
#define CPUID_TYPE_THERMAL 6U
#define CPUID_TYPE_STRUCTURED 7U
#define CPUID_TYPE_EXTENDED 0x80000000U
#define CPUID_TYPE_EXTENDED_FEATURES 0x80000001U
#define CPUID_TYPE_BRAND 0x80000002U
#define CPUID_TYPE_BRAND_MORE 0x80000003U
#define CPUID_TYPE_BRAND_END 0x80000004U
#define CPUID_TYPE_ADDRESS_SIZE 0x80000008U

#define CPUID_TYPE_INTEL_APIC_OLD 0x04U
#define CPUID_TYPE_INTEL_APIC_SHIFT 0xBU

#include <Types.h>

namespace x64 {
    enum class CPUVendorEnum : u64
    {
        UNKNOWN   = 0,
        INTEL     = 0x756e6547, // "GenuineIntel"
        AMD       = 0x68747541, // "AuthenticAMD"
        CRYIX     = 0x69727943, // "CyrixInstead"
        VIA       = 0x20414956, // "VIA VIA VIA "
        TRANSMETA = 0x756e6547, // "GenuineTMx86"
        CENTAUR   = 0x746e6543, // "CentaurHauls"
        NEXGEN    = 0x6e6f6547, // "NexGenDriven"
        RISE      = 0x65736952, // "RiseRiseRise"
        SIS       = 0x49505353, // "SiS SiS SiS "
        UMC       = 0x20434d55, // "UMC UMC UMC "
        NSC       = 0x736c7561, // "Geode by NSC"
        HYGON     = 0x6e6f6779, // "HygonGenuine"
        ZHAOXIN   = 0x6e616853, // "  Shanghai  "
        HEDT      = 0x20427953, // "  SysBios  "
        ELBRUS    = 0x20444b45, // "E2K MACHINE "

        QEMU          = 0x47435447, // "TCGTCGTCGTCG"
        KVM           = 0x4d564b20, // " KVMKVMKVM  "
        VMWARE        = 0x61774d56, // "VMwareVMware"
        VIRTUALBOX    = 0x6f784256, // "VBoxVBoxVBox"
        XEN           = 0x4d4d564e, // "XenVMMXenVMM"
        HYPERV        = 0x726f6369, // "Microsoft Hv"
        PARALLELS     = 0x70726c20, // " prl hyperv "
        PARALLELS_ALT = 0x7079726c, // " lrpepyh vr "
        BHYVE         = 0x65766268, // "bhyve bhyve "
        QNX           = 0x53514d56, // " QNXQVMBSQG "
    };

    // MARK: - [CPUID]

    template <u32 Index>
    struct CPUIdentifyPack
    {
        u32 EAX, EBX, ECX, EDX;

        CPUIdentifyPack(u32 a, u32 b, u32 c, u32 d)
            : EAX(a)
            , EBX(b)
            , ECX(c)
            , EDX(d)
        {
        }
    };

    // MARK: - [VENDOR]

    template <>
    struct CPUIdentifyPack<CPUID_TYPE_VENDOR>
    {
        u32           MaxQueryNum;
        CPUVendorEnum Vendor;

        CPUIdentifyPack(u32 a, u32 b, u32 c, u32 d)
            : MaxQueryNum(a)
            , Vendor(static_cast<CPUVendorEnum>(b))
        {
        }
    };

    // MARK: - [FEATURES]

    template <>
    struct CPUIdentifyPack<CPUID_TYPE_FEATURES>
    {
        union
        {
            u32 EAX;
            struct
            {
                u32 Stepping : 4;
                u32 Model : 4;
                u32 Family : 4;
                u32 __reserved__0 : 4;
                u32 ModelExtended : 4;
                u32 FamilyExtended : 8;
                u32 __reserved__1 : 4;
            } __attribute__((packed));
        };
        union
        {
            u32 EBX;
            struct
            {
                u32 Brand : 8;
                u32 CacheLineSize : 8;
                /**
                 * @brief Maximum number of addressable IDs for logical
                 * processors
                 *
                 * Should round up to the power of 2. This value available if
                 * the hyperthreading is enabled (EDX[28] is True).
                 */
                u32 MaxLogicalProcessorAmount : 8;
                /**
                 * @brief Initial APIC ID
                 *
                 * EBX[31:24] saves the initial APIC ID. If the query vector 0xB
                 * is supported, the topology of the processor is also
                 * supported, the value should be replaced by [x2APIC ID] in
                 * extended topology.
                 */
                u32 InitialApicId : 8;
            } __attribute__((packed));
        };
        union
        {
            u32 ECX;
            struct
            {
                u32 SSE3 : 1;
                u32 PCLMUL : 1;
                u32 DTES64 : 1;
                u32 MONITOR : 1;
                u32 DS_CPL : 1;
                u32 VMX : 1;
                u32 SMX : 1;
                u32 EST : 1;
                u32 TM2 : 1;
                u32 SSSE3 : 1;
                u32 CID : 1;
                u32 __reserved__2 : 1;
                u32 FMA : 1;
                u32 CX16 : 1;
                u32 ETPRD : 1;
                u32 PDCM : 1;
                u32 __reserved__3 : 1;
                u32 PCIDE : 1;
                u32 DCA : 1;
                u32 SSE4_1 : 1;
                u32 SSE4_2 : 1;
                u32 x2APIC : 1;
                u32 MOVBE : 1;
                u32 POPCNT : 1;
                u32 __reserved__4 : 1;
                u32 AES : 1;
                u32 XSAVE : 1;
                u32 OSXSAVE : 1;
                u32 AVX : 1;
            } __attribute__((packed));
        };
        union
        {
            u32 EDX;
            struct
            {
                u32 FPU : 1;
                u32 VME : 1;
                u32 DE : 1;
                u32 PSE : 1;
                u32 TSC : 1;
                u32 MSR : 1;
                u32 PAE : 1;
                u32 MCE : 1;
                u32 CX8 : 1;
                u32 APIC : 1;
                u32 __reserved__5 : 1;
                u32 SEP : 1;
                u32 MTRR : 1;
                u32 PGE : 1;
                u32 MCA : 1;
                u32 CMOV : 1;
                u32 PAT : 1;
                u32 PSE36 : 1;
                u32 PSN : 1;
                u32 CLF : 1;
                u32 __reserved__6 : 1;
                u32 DTES : 1;
                u32 ACPI : 1;
                u32 MMX : 1;
                u32 FXSR : 1;
                u32 SSE : 1;
                u32 SSE2 : 1;
                u32 SS : 1;
                u32 HTT : 1;
                u32 TM1 : 1;
                u32 IA64 : 1;
                u32 PBE : 1;
            } __attribute__((packed));
        };

        CPUIdentifyPack(u32 a, u32 b, u32 c, u32 d)
            : EAX(a)
            , EBX(b)
            , ECX(c)
            , EDX(d)
        {
        }
    };

    // MARK: - [CACHE]

    template <>
    struct CPUIdentifyPack<CPUID_TYPE_CACHE>
    {
        union
        {
            u32 EAX;
            struct
            {
                u32 CacheType : 5;
                u32 CacheLevel : 3;
                u32 SelfInitializing : 1;
                u32 FullyAssociative : 1;
                u32 __reserved__0 : 4;
                u32 MaximumAddressableIds : 12;
                u32 MaximumAddressableIdsMask : 12;
            } __attribute__((packed));
        };
        union
        {
            u32 EBX;
            struct
            {
                u32 LineSize : 12;
                u32 LineSizeMask : 12;
                u32 Associativity : 10;
                u32 AssociativityMask : 10;
                u32 __reserved__1 : 2;
                u32 Sets : 32;
            } __attribute__((packed));
        };
        union
        {
            u32 ECX;
            struct
            {
                u32 Partitions : 10;
                u32 PartitionsMask : 10;
                u32 Ways : 10;
                u32 WaysMask : 10;
                u32 SetsMask : 32;
            } __attribute__((packed));
        };
        union
        {
            u32 EDX;
            struct
            {
                u32 WriteBack : 1;
                u32 Inclusive : 1;
                u32 ComplexIndexing : 1;
                u32 __reserved__2 : 29;
            } __attribute__((packed));
        };

        CPUIdentifyPack(u32 a, u32 b, u32 c, u32 d)
            : EAX(a)
            , EBX(b)
            , ECX(c)
            , EDX(d)
        {
        }
    };

    // MARK: - [SERIAL]

    template <>
    struct CPUIdentifyPack<CPUID_TYPE_SERIAL>
    {
        union
        {
            u32 EAX;
            struct
            {
                u32 MaximumInputValue : 32;
            } __attribute__((packed));
        };
        union
        {
            u32 EBX;
            struct
            {
                u32 SerialNumber0 : 32;
            } __attribute__((packed));
        };
        union
        {
            u32 ECX;
            struct
            {
                u32 SerialNumber1 : 32;
            } __attribute__((packed));
        };
        union
        {
            u32 EDX;
            struct
            {
                u32 SerialNumber2 : 32;
            } __attribute__((packed));
        };

        CPUIdentifyPack(u32 a, u32 b, u32 c, u32 d)
            : EAX(a)
            , EBX(b)
            , ECX(c)
            , EDX(d)
        {
        }
    };

    // MARK: - [MONITOR]

    template <>
    struct CPUIdentifyPack<CPUID_TYPE_MONITOR>
    {
        union
        {
            u32 EAX;
            struct
            {
                u32 SmallestMonitorLineSize : 32;
            } __attribute__((packed));
        };
        union
        {
            u32 EBX;
            struct
            {
                u32 LargestMonitorLineSize : 32;
            } __attribute__((packed));
        };
        union
        {
            u32 ECX;
            struct
            {
                u32 MonitorType : 32;
            } __attribute__((packed));
        };
        union
        {
            u32 EDX;
            struct
            {
                u32 MonitorInterrupts : 32;
            } __attribute__((packed));
        };

        CPUIdentifyPack(u32 a, u32 b, u32 c, u32 d)
            : EAX(a)
            , EBX(b)
            , ECX(c)
            , EDX(d)
        {
        }
    };

    // MARK: - [THERMAL]

    template <>
    struct CPUIdentifyPack<CPUID_TYPE_THERMAL>
    {
        union
        {
            u32 EAX;
            struct
            {
                u32 DigitalThermalSensorEnabled : 1;
                u32 __reserved__0 : 31;
            } __attribute__((packed));
        };
        union
        {
            u32 EBX;
            u32 __reserved__1;
        };
        union
        {
            u32 ECX;
            u32 __reserved__2;
        };
        union
        {
            u32 EDX;
            u32 __reserved__3;
        };

        CPUIdentifyPack(u32 a, u32 b, u32 c, u32 d)
            : EAX(a)
            , EBX(b)
            , ECX(c)
            , EDX(d)
        {
        }
    };

    // MARK: - [STRUCTURED]

    template <>
    struct CPUIdentifyPack<CPUID_TYPE_STRUCTURED>
    {
        union
        {
            u32 EAX;
            struct
            {
                u32 NumberOfSubleafs : 32;
            } __attribute__((packed));
        };
        union
        {
            u32 EBX;
            u32 __reserved__0;
        };
        union
        {
            u32 ECX;
            u32 __reserved__1;
        };
        union
        {
            u32 EDX;
            u32 __reserved__2;
        };

        CPUIdentifyPack(u32 a, u32 b, u32 c, u32 d)
            : EAX(a)
            , EBX(b)
            , ECX(c)
            , EDX(d)
        {
        }
    };

    // MARK: - [EXTENDED]

    template <>
    struct CPUIdentifyPack<CPUID_TYPE_EXTENDED>
    {
        u32 EAX, EBX, ECX, EDX;

        CPUIdentifyPack(u32 a, u32 b, u32 c, u32 d)
            : EAX(a)
            , EBX(b)
            , ECX(c)
            , EDX(d)
        {
        }
    };

    // MARK: - [EXTENDED_FEATURES]

    template <u32 Index, u8 SubIndex = 0>
    CPUIdentifyPack<Index> ReadCPUIdentifyPack()
    {
        u32 regs[4];
        if constexpr (SubIndex != 0)
            __asm__ __volatile__(
                "cpuid"
                : "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
                : "a"(Index));
        else
            __asm__ __volatile__(
                "cpuid"
                : "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
                : "a"(Index), "c"(SubIndex));
        return CPUIdentifyPack<Index>(regs[0], regs[1], regs[2], regs[3]);
    }
}