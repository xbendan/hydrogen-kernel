#pragma once

#include <Types.h>

namespace System::Collections {
    template <typename T>
    class Vector
    {
    public:
        Vector(usize capacity)
            : m_data(new T[capacity])
            , m_capacity(capacity)
            , m_count(0)
        {
        }
        Vector(Vector const& other)
            : m_data(new T[other.m_capacity])
            , m_capacity(other.m_capacity)
            , m_count(other.m_count)
        {
            for (usize i = 0; i < m_count; i++)
                m_data[i] = other.m_data[i];
        }
        Vector(Vector&& other)
            : m_data(other.m_data)
            , m_capacity(other.m_capacity)
            , m_count(other.m_count)
        {
            other.m_data     = nullptr;
            other.m_capacity = 0;
            other.m_count    = 0;
        }
        ~Vector() { delete[] m_data; }

        Vector& operator=(Vector const& other)
        {
            if (this != &other) {
                delete[] m_data;
                m_data     = new T[other.m_capacity];
                m_capacity = other.m_capacity;
                m_count    = other.m_count;
                for (usize i = 0; i < m_count; i++)
                    m_data[i] = other.m_data[i];
            }
            return *this;
        }

        Vector& operator=(Vector&& other)
        {
            if (this != &other) {
                delete[] m_data;
                m_data           = other.m_data;
                m_capacity       = other.m_capacity;
                m_count          = other.m_count;
                other.m_data     = nullptr;
                other.m_capacity = 0;
                other.m_count    = 0;
            }
            return *this;
        }

        T& operator[](usize index) { return m_data[index]; }

        T const& operator[](usize index) const { return m_data[index]; }

        void Ensure(usize capacity)
        {
            if (m_capacity < capacity) {
                usize newCapacity = m_capacity * 2;
                if (newCapacity < capacity)
                    newCapacity = capacity;

                T*    newData = new T[newCapacity];
                usize newSize = m_count < newCapacity ? m_count : newCapacity;
                for (usize i = 0; i < newSize; i++)
                    newData[i] = m_data[i];
                delete[] m_data;
                m_data     = newData;
                m_capacity = newCapacity;
                m_count    = newSize;
            }
        }

        void Ensure() { Ensure(m_count + 1); }

        void Truncate(usize size)
        {
            if (size < m_count)
                m_count = size;
        }

        void PushBack(T const& value)
        {
            Ensure();
            m_data[m_count++] = value;
        }

        T PopBack() { return m_data[--m_count]; }

        usize Count() const { return m_count; }

    private:
        T*    m_data;
        usize m_capacity;
        usize m_count;
    };
}