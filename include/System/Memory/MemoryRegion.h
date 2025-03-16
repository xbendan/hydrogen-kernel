#pragma once

#include <Meta/Property.h>
#include <Meta/Slice.h>
#include <System/Collections/LinkedList.h>
#include <System/Numerics/Align.h>
#include <System/Numerics/Math.h>
#include <Types.h>

namespace System::MemoryService {
    using namespace Numerics;

    struct MemoryRegion
    {
        template <typename T = byte>
        struct _It;

        uptr _begin, _end;

        auto begin();
        auto end();

        template <typename T = byte>
        T* GetAtIndex(usize index)
        {
            return reinterpret_cast<T*>(_begin + sizeof(T) * index);
        }

        template <typename T = byte>
        T* GetAtIndex(usize index) const
        {
            return reinterpret_cast<T*>(_begin + sizeof(T) * index);
        }

        template <typename T = byte>
        void Set(T const& value, usize index)
        {
            *GetAtIndex<T>(index) = value;
        }

        template <typename T = byte>
        MemoryRegion& Set(T const& value)
        {
            for (usize i = 0; i < Length() / sizeof(T); i++) {
                Set(i, value);
            }
            return *this;
        }

        MemoryRegion& Zeroize()
        {
            Set(0);
            return *this;
        }

        bool Compare(MemoryRegion const& other) const
        {
            // TODO: Optimize this
            for (usize i = 0; i < (_end - _begin); i++) {
                if (GetAtIndex(i) != other.GetAtIndex(i)) {
                    return false;
                }
            }

            return true;
        }

        template <typename T>
        T* FindValue(T const& value) const
        {
            for (usize i = 0; i < 0 / sizeof(T); i++) {
                if (GetAtIndex<T>(i) == value) {
                    return GetAtIndex<T>(i);
                }
            }

            return nullptr;
        }

        template <typename T>
        bool FindValueExists(T const& value) const
        {
            return FindValue(value) != nullptr;
        }

        MemoryRegion& GetAsInnerAligned(usize alignment)
        {
            AlignUp(_begin, alignment);
            AlignDown(_end, alignment);

            return *this;
        }

        MemoryRegion& GetAsOuterAligned(usize alignment)
        {
            AlignDown(_begin, alignment);
            AlignUp(_end, alignment);

            return *this;
        }

        MemoryRegion& GetAsRelocated(uptr address)
        {
            _begin = address;

            return *this;
        }

        MemoryRegion& GetAsMoved(usize offset)
        {
            _begin += offset;
            _end += offset;

            return *this;
        }

        MemoryRegion GetAsIntersected(MemoryRegion const& other) const
        {
            return MemoryRegion{ _begin,
                                 _end < other._end ? _end : other._end };
        }

        template <typename T = byte>
        T* operator[](usize index)
        {
            return GetAtIndex<T>(index);
        }

        template <typename T = byte>
        T* operator[](usize index) const
        {
            return GetAtIndex<T>(index);
        }

        always_inline constexpr Bytes GetAsByteSlice() const
        {
            return Bytes{ (byte*)_begin, _end - _begin };
        }

        usize Length() const { return _end - _begin; }

        byte* GetBeginAsPointed() const { return (byte*)_begin; }
    };

    template <typename T>
    struct MemoryRegion::_It
    {
        T*    _ptr;
        usize _size;
        usize _curr;

        _It(T* ptr, usize _size, usize curr)
            : _ptr(ptr)
            , _size(_size)
            , _curr(curr)
        {
        }

        _It& operator++()
        {
            if (sizeof(T) * _curr < _size) {
                _curr++;
            }

            return *this;
        }

        T& operator*() { return *reinterpret_cast<T*>(_ptr); }
        T* operator->() { return reinterpret_cast<T*>(_ptr); }

        bool operator!=(Empty) const { return sizeof(T) * _curr < _size; }

        bool operator==(_It const& it) const { return _ptr == it._ptr; }
        bool operator!=(_It const& it) const { return _ptr != it._ptr; }
    };

    auto MemoryRegion::begin()
    {
        return _It<>(reinterpret_cast<byte*>(_begin), Length(), 0);
    }

    auto MemoryRegion::end()
    {
        return Empty{};
    }

    template <typename T, typename U = T>
        requires(Std::isIntegral<T> or Std::isPointer<T> and Std::isIntegral<U>)
    MemoryRegion MakeRegionAt(T t, U u)
    {
        return MemoryRegion{ reinterpret_cast<uptr>(t), u };
    }
}