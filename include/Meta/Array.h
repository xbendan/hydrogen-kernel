#pragma once

#include <Assertion.h>
#include <InitializerList.h>
#include <Meta/Property.h>
#include <Types.h>
#include <Utility.h>

namespace Meta {
    template <typename, usize = 0>
    class Array;

    template <typename T, usize TLen>
    class Array
    {
    public:
        using InnerType = T;

        always_inline constexpr Array() {}

        always_inline constexpr Array(T const (&data)[TLen])
            : _data{ data }
        {
        }

        always_inline constexpr Array(InitializerList<T> list)
        {
            Requires(list.size() == TLen);

            usize i = 0;
            for (auto& item : list) {
                _data[i++] = Std::move(item);
            }
        }

        always_inline constexpr Array(Array const& other)
            : _data{ other._data }
        {
        }

        always_inline constexpr Array(Array&& other)
            : _data{ Std::move(other._data) }
        {
        }

        always_inline constexpr Array& operator=(Array const& other)
        {
            _data = other._data;
            return *this;
        }

        always_inline constexpr Array& operator=(Array&& other)
        {
            _data = Std::move(other._data);
            return *this;
        }

        always_inline constexpr Array& operator=(InitializerList<T> list)
        {
            Requires(list.size() == TLen);

            usize i = 0;
            for (auto& item : list) {
                _data[i++] = Std::move(item);
            }

            return *this;
        }

        always_inline constexpr T& operator[](usize index)
        {
            assert(index < TLen, "Index out of bounds");
            return _data[index];
        }

        always_inline constexpr T const& operator[](usize index) const
        {
            assert(index < TLen, "Index out of bounds");
            return _data[index];
        }

        always_inline constexpr operator T*() const { return _data; }

        always_inline constexpr usize Length() const { return TLen; }

        always_inline constexpr T* begin() { return _data; }

        always_inline constexpr T* end() { return _data + TLen; }

    private:
        T _data[TLen];
    };

    template <typename T>
    class Array<T, 0>
    {
    public:
        using InnerType = T;

        Array() = default;
        Array(usize size)
            : _data(new T[size])
            , _size{ size }
        {
        }
        Array(T* data, usize size)
            : _data(data)
            , _size{ size }
        {
        }
        Array(Array const& other)
        {
            if (_data != Empty{}) {
                delete[] _data;
            }

            _data = new T[other._size];
            _size = other._size;
            for (usize i = 0; i < _size; i++) {
                _data[i] = other._data[i];
            }
        }
        Array(Array&& other)
            : _data(other._data)
            , _size{ other._size }
        {
            other._data = nullptr;
            other._size = 0;
        }
        Array(InitializerList<T> list)
            : _data(new T[list.size()])
            , _size{ list.size() }
        {
            usize i = 0;
            for (auto& item : list) {
                _data[i++] = Std::move(item);
            }
        }

        ~Array() { delete[] _data; }

        T& operator[](usize index) { return _data[index]; }

        T const& operator[](usize index) const { return _data[index]; }

        void operator=(Array const& other)
        {
            delete[] _data;
            _data = new T[other._size];
            _size = other._size;
            for (usize i = 0; i < _size; i++) {
                _data[i] = other._data[i];
            }
        }

        void operator=(Array&& other)
        {
            delete[] _data;

            _data       = other._data;
            _size       = other._size;
            other._data = nullptr;
            other._size = 0;
        }

        T* operator*() { return _data; }

        usize Length() const { return _size; }

        constexpr T* begin() { return _data; }

        constexpr T* end() { return _data + _size; }

    private:
        T*    _data = nullptr;
        usize _size = 0;
        // usize _size = 0;
    };

    struct Arrays
    {
        template <typename T>
        static void Fill(T* ptr, usize len, T const& value)
        {
            for (usize i = 0; i < len; ++i) {
                ptr[i] = value;
            }
        }

        template <typename T>
        static void Fill(Array<T>& array, T const& value)
        {
            Fill(*array, array.Length(), value);
        }

        template <typename T>
        static void Copy(T const* src, T* dst, usize len)
        {
            for (usize i = 0; i < len; ++i) {
                dst[i] = src[i];
            }
        }

        template <typename T>
        static void
        Copy(T* src, usize srcIndex, T* dst, usize dstIndex, usize len)
        {
            for (usize i = 0; i < len; ++i) {
                dst[dstIndex + i] = src[srcIndex + i];
            }
        }

        template <typename T>
        static void Reverse(T* ptr, usize len)
        {
            for (usize i = 0; i < len / 2; ++i) {
                T temp           = Std::move(ptr[i]);
                ptr[i]           = Std::move(ptr[len - 1 - i]);
                ptr[len - 1 - i] = Std::move(temp);
            }
        }

        template <typename T>
        static void Reverse(Array<T>& array)
        {
            Reverse(array.buf(), array.len());
        }
    };

    template <typename T>
    struct ArrayEnumerator
    {
        using InnerType  = T;
        using ResultType = T&;

        T*    _data;
        usize _size;
        usize _index;

        ArrayEnumerator(T* data, usize size)
            : _data(data)
            , _size(size)
            , _index(0)
        {
        }

        ArrayEnumerator(T const* data, usize size)
            : _data(const_cast<T*>(data))
            , _size(size)
            , _index(0)
        {
        }

        always_inline T& operator*() { return _data[_index]; }

        always_inline ArrayEnumerator<T>& operator++()
        {
            _index++;
            return *this;
        }

        always_inline bool operator!=(Empty) { return _index < _size; }

        always_inline void operator=(Empty) { _index = 0; }
    };
}

using Meta::Array;
using Meta::ArrayEnumerator;
using Meta::Arrays;