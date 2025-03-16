#define MSR_APIC_BASE 0x1b
#define MSR_EFER 0xC0000080
#define MSR_STAR 0xC0000081
#define MSR_LSTAR 0xC0000082
#define MSR_CSTAR 0xC0000083
#define MSR_SYSCALL_MASK 0xC0000084
#define MSR_FS_BASE 0xC0000100
#define MSR_USER_GS_BASE 0xC0000101
#define MSR_KERN_GS_BASE 0xC0000102
#define MSR_TSC_AUX 0xC0000103

#define CR0_PROTECTION_MODE (1 << 0)
#define CR0_MONITOR_COPROCESSOR (1 << 1)
#define CR0_EMULATE_COPROCESSOR (1 << 2)
#define CR0_TASK_SWITCHED (1 << 3)
#define CR0_EXTENSION_TYPE (1 << 4)
#define CR0_NUMERIC_ERROR (1 << 5)
#define CR0_WRITE_PROTECT (1 << 16)
#define CR0_ALIGNMENT_MASK (1 << 18)
#define CR0_NOT_WRITE_THROUGH (1 << 29)
#define CR0_CACHE_DISABLE (1 << 30)
#define CR0_PAGING (1 << 31)

#define CR4_V8086_MODE (1 << 0)
#define CR4_PMODE_VIRT_INT (1 << 1)
#define CR4_TIME_STAMP_DISABLE (1 << 2)
#define CR4_DEBUG_EXT (1 << 3)
#define CR4_PAGE_SIZE_EXT (1 << 4)
#define CR4_PHYS_ADDR_EXT (1 << 5)
#define CR4_MACHINE_CHECK (1 << 6)
#define CR4_PAGE_GLOBAL_ENABLE (1 << 7)
#define CR4_PERFMONITOR (1 << 8)
#define CR4_OSFXSR (1 << 9)
#define CR4_OSXMMEXCPT (1 << 10)
#define CR4_USER_MODE_INSTRUCTION_PREVENTION (1 << 11)
#define CR4_LINEAR_ADDR_57BIT (1 << 12)
#define CR4_VIRT_MACHINE_EXT (1 << 13)
#define CR4_SAFER_MODE_EXT (1 << 14)
#define CR4_FSGSBASE                                                           \
    (1 << 16) // Enables the instructions RDFSBASE, RDGSBASE, WRFSBASE, and
              // WRGSBASE
#define CR4_PCID (1 << 17)    // Enables the process-context identifiers (PCIDs)
#define CR4_OSXSAVE (1 << 18) // XSAVE and Processor Extended States Enable Bit
#define CR4_SUPERVISOR_EXEC_PROTECTION (1 << 20)
#define CR4_SUPERVISOR_ACCESS_PREVENTION (1 << 21)
#define CR4_PROTECTION_KEY_ENABLE (1 << 22)
#define CR4_CONTROL_FLOW_ENFORCEMENT (1 << 23)
#define CR4_PROTECTION_KEY_SUPERVISOR (1 << 24)

#include <Types.h>

namespace x64 {
    struct Registers
    {
        u64 r15, r14, r13, r12, r11, r10, r9, r8;
        u64 rbp, rdi, rsi, rdx, rcx, rbx, rax;
        u64 err;
        u64 rip, cs, rflags, rsp, ss;
    } __attribute__((packed));
}