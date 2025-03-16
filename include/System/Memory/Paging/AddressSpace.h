#include <Meta/Flags.h>
#include <Meta/Result.h>
#include <System/Hal/MemoryFlags.h>
#include <System/Hal/MemoryPaging.h>
#include <System/Memory/MemoryRegion.h>

namespace System::MemoryService {
    template <bool = false>
    class AddressSpace;

    template <>
    class AddressSpace<false> : Hal::PagingData<false>
    {
    public:
        MemoryRegion AllocateVirtMemory4K(usize);

        MemoryRegion AllocateVirtMemory2M(usize);

        void FreeVirtMemory4K(uptr, usize);

        void FreeVirtMemory2M(uptr, usize);

        void MapAddress4K(
            uptr,
            uptr,
            usize,
            Flags<Hal::VmmFlags> = Hal::VmmFlags::USER_RW);

        void MapAddress2M(
            uptr,
            uptr,
            usize,
            Flags<Hal::VmmFlags> = Hal::VmmFlags::USER_RW);

        void UnmapAddress4K(u64, usize);

        void UnmapAddress2M(u64, usize);

        /**
         * @brief Provide access to page table entries
         *
         * @return Reference to the page table entry
         */
        Hal::VmmEntry& operator[](u64);

        Hal::VmmEntry const& operator[](u64) const;

        /**
         * @brief Translate a virtual address to a physical address
         *
         * @return uptr
         */
        uptr operator()(uptr);

    private:
    };

    template <>
    class AddressSpace<true> : Hal::PagingData<true>
    {
        uptr AllocateVirtMemory4K(usize);

        uptr AllocateVirtMemory2M(usize);

        void FreeVirtMemory4K(u64, usize);

        void FreeVirtMemory2M(u64, usize);

        void MapAddress4K(
            uptr,
            uptr,
            usize,
            Flags<Hal::VmmFlags> = Hal::VmmFlags::USER_RW);

        void MapAddress2M(
            uptr,
            uptr,
            usize,
            Flags<Hal::VmmFlags> = Hal::VmmFlags::USER_RW);

        void UnmapAddress4K(u64, usize);

        void UnmapAddress2M(u64, usize);

        /**
         * @brief Provide access to page table entries
         *
         * @return Reference to the page table entry
         */
        Hal::VmmEntry& operator[](u64);

        Hal::VmmEntry const& operator[](u64) const;

        /**
         * @brief Translate a virtual address to a physical address
         *
         * @return uptr
         */
        uptr operator()(uptr);
    };

    using KernelAddressSpace = AddressSpace<true>;
    using UserAddressSpace   = AddressSpace<false>;
}