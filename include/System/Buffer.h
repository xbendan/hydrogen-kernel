#pragma once

#include <Meta/Manual.h>
#include <Meta/Property.h>
#include <Meta/Slice.h>
#include <System/Numerics/Math.h>
#include <Terminate.h>
#include <Types.h>

namespace System {
    template <typename T, usize TLen = 0>
    struct Buffer;

    template <typename T>
    struct Buffer<T, 0>
    {
        using InnerType = T;

        Manual<T>* _data{ nullptr };
        usize      Capacity{ 0 };
        usize      Length{ 0 };

        Buffer(usize cap = 0) { Ensure(cap); }

        Buffer(T* data, usize cap, usize len = 0)
            : _data(data)
            , Capacity(cap)
            , Length(len)
        {
        }

        Buffer(Buffer const& other)
        {
            Ensure(other.Length);

            Length = other.Length;
            for (usize i = 0; i < other.Length; ++i) {
                _data[i](other._data[i].get());
            }
        }

        Buffer(Buffer&& other)
            : _data(other._data)
            , Capacity(other.Capacity)
            , Length(other.Length)
        {
            other._data    = nullptr;
            other.Capacity = 0;
            other.Length   = 0;
        }

        ~Buffer() { Dispose(); }

        Buffer& operator=(Buffer const& other)
        {
            if (this == &other) {
                return *this;
            }

            if (_data) {
                for (usize i = 0; i < Length; ++i) {
                    ~_data[i]();
                }
                delete[] _data;
            }

            Ensure(other.Length);

            Length = other.Length;
            for (usize i = 0; i < other.Length; ++i) {
                _data[i](other._data[i].get());
            }

            return *this;
        }

        Buffer& operator=(Buffer&& other)
        {
            if (this == &other) {
                return *this;
            }
            Dispose();

            _data    = other._data;
            Capacity = other.Capacity;
            Length   = other.Length;

            other._data    = nullptr;
            other.Capacity = 0;
            other.Length   = 0;

            return *this;
        }

        T& operator[](usize i)
        {
            if (i >= Length) [[unlikely]] {
                Std::terminate("Buffer::operator[]: index out of bounds");
            }

            return _data[i].Get();
        }

        T const& operator[](usize i) const
        {
            if (i >= Length) [[unlikely]] {
                Std::terminate("Buffer::operator[]: index out of bounds");
            }

            return _data[i].Get();
        }

        T const& operator+=(T const& val);

        void operator--();

        usize Ensure(usize len)
        {
            if (len <= Capacity) {
                return Capacity;
            }

            if (not _data) {
                _data    = new Manual<T>[len];
                Capacity = len;
                return Capacity;
            }

            usize n = Numerics::Max(Capacity * 2, len);

            Manual<T>* data = new Manual<T>[n];
            for (usize i = 0; i < Length; ++i) {
                data[i](_data[i].Take());
                ~_data[i]();
            }

            delete[] _data;
            _data    = data;
            Capacity = n;
        }

        usize Resize(usize len, T fill = {})
        {
            if (len == Length || not len) {
                return Length;
            }

            if (len > Capacity) {
                Std::terminate("Buffer::Resize: capacity exceeded");
            }

            for (usize i = 0; i < len; ++i) {
                if (i < Length) {
                    _data[i] = _data[i];
                } else {
                    _data[i] = fill;
                }
            }

            Length = len;

            return Length;
        }

        void Truncate(usize len)
        {
            if (len >= Length) {
                return;
            }

            for (usize i = len; i < Length; ++i) {
                ~_data[i]();
            }

            Length = len;
        }

        void Dispose()
        {
            if (_data) {
                for (usize i = 0; i < Length; ++i) {
                    ~_data[i]();
                }
                delete[] _data;
            }
        }

        template <typename... Args>
        void EmplaceAt(usize i, Args&&... args)
        {
            if (i >= Length) [[unlikely]] {
                Std::terminate("Buffer::EmplaceAt: index out of bounds");
            }

            _data[i](Std::forward<Args>(args)...);
        }

        void InsertAt(usize i, T const& val)
        {
            if (i >= Length) [[unlikely]] {
                Std::terminate("Buffer::InsertAt: index out of bounds");
            }

            if (Length == Capacity) {
                Std::terminate("Buffer::InsertAt: capacity exceeded");
            }

            for (usize j = Length; j > i; --j) {
                _data[j] = _data[j - 1];
            }

            _data[i] = val;
            ++Length;
        }

        T* Take()
        {
            if (not _data) {
                return nullptr;
            }

            T* result = _data;

            _data    = nullptr;
            Capacity = 0;
            Length   = 0;

            return result;
        }

        always_inline constexpr T* begin()
        {
            return _data ? &(_data->Get()) : nullptr;
        }

        always_inline constexpr Empty end() { return Empty{}; }
    };

    template <typename T, usize TLen>
    struct Buffer
    {
        using InnerType = T;

        Manual<T>   Data[TLen];
        usize const Capacity = TLen;
        usize       Length{ 0 };

        Buffer() = default;

        Buffer(Buffer<T, TLen> const& other)
        {
            for (usize i = 0; i < other.Length; ++i) {
                Data[i](other.Data[i].Get());
            }
            Length = other.Length;
        }

        Buffer(Buffer&& other)
            : Length(other.Length)
        {
            for (usize i = 0; i < other.Length; ++i) {
                Data[i](other.Data[i].Take());
            }
            other.Length = 0;
        }

        ~Buffer() = default;

        Buffer& operator=(Buffer const& other)
        {
            for (usize i = 0; i < other.Length; ++i) {
                Data[i](other.Data[i].Get());
            }
            Length = other.Length;

            return *this;
        }

        Buffer& operator=(Buffer&& other)
        {
            for (usize i = 0; i < other.Length; ++i) {
                Data[i](other.Data[i].Take());
            }
            Length       = other.Length;
            other.Length = 0;

            return *this;
        }

        T& operator[](usize i)
        {
            if (i >= Length) {
                Std::terminate("Buffer::operator[]: index out of bounds");
            }

            return Data[i].Get();
        }

        T const& operator[](usize i) const
        {
            if (i >= Length) {
                Std::terminate("Buffer::operator[]: index out of bounds");
            }

            return Data[i].Get();
        }

        T const& operator+=(T const& val)
        {
            if (Length == Capacity) {
                Std::terminate("Buffer::operator+=: capacity exceeded");
            }

            Data[Length] = Std::move(val);
            ++Length;

            return val;
        }

        void operator--()
        {
            if (Length) {
                ~Data[Length - 1]();
                --Length;
            }
        }

        template <typename... Args>
        void EmplaceAt(usize i, Args&&... args)
        {
            if (i >= Length) {
                Std::terminate("Buffer::EmplaceAt: index out of bounds");
            }

            Data[i](Std::forward<Args>(args)...);
        }

        void InsertAt(usize i, T const& val)
        {
            if (i >= Length) {
                Std::terminate("Buffer::InsertAt: index out of bounds");
            }

            if (Length == Capacity) {
                Std::terminate("Buffer::InsertAt: capacity exceeded");
            }

            for (usize j = Length; j > i; --j) {
                Data[j] = Data[j - 1];
            }

            Data[i] = Std::move(val);
            ++Length;
        }

        always_inline constexpr T* begin() { return Data; }

        always_inline constexpr Empty end() { return Empty{}; }
    };
}