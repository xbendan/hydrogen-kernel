#pragma once

#include <System/Collections/ICollection.h>
#include <Types.h>

namespace System::Collections {

    // MARK: - List

    template <typename T = Empty>
    class LinkedList;

    // MARK: - Empty Specialization

    template <>
    class LinkedList<Empty>
    {
    public:
        template <typename T>
        class Linker;
    };

    template <typename T>
    struct LinkedList<Empty>::Linker
    {
        using PointedType = LinkedList<Empty>::Linker<T>*;

        PointedType _previous, _next;

        T* CastAs() { return static_cast<T*>(this); }
    };

    // MARK: - No Specialization

    template <typename T>
    class LinkedList
    {
    protected:
        struct _Node;
        struct _It;

        friend struct _It;

    public:
        LinkedList()
            : m_head(nullptr)
            , m_tail(nullptr)
            , m_count(0)
        {
        }

        LinkedList(LinkedList<T> const& other)
            : m_head(nullptr)
            , m_tail(nullptr)
            , m_count(0)
        {
            for (auto const& item : other) {
                add(item);
            }
        }

        LinkedList(LinkedList<T>&& other)
            : m_head(other.m_head)
            , m_tail(other.m_tail)
            , m_count(other.m_count)
        {
            other.m_head  = nullptr;
            other.m_tail  = nullptr;
            other.m_count = 0;
        }

        // MARK: - Collections

        void add(T const& value)
        {
            auto node = new _Node{ nullptr, nullptr, value };

            if (m_head == nullptr) {
                m_head = node;
                m_tail = node;
            } else {
                m_tail->m_next   = node;
                node->m_previous = m_tail;
                m_tail           = node;
            }

            m_count++;
        }

        void remove(T const& value)
        {
            for (auto node = m_head; node != nullptr; node = node->m_next) {
                if (node->m_value == value) {
                    if (node->m_previous != nullptr) {
                        node->m_previous->m_next = node->m_next;
                    } else {
                        m_head = node->m_next;
                    }

                    if (node->m_next != nullptr) {
                        node->m_next->m_previous = node->m_previous;
                    } else {
                        m_tail = node->m_previous;
                    }

                    delete node;
                    m_count--;
                    break;
                }
            }
        }

        void clear()
        {
            for (auto node = m_head; node != nullptr;) {
                auto next = node->m_next;
                delete node;
                node = next;
            }

            m_head  = nullptr;
            m_tail  = nullptr;
            m_count = 0;
        }

        // MARK: - Iteration

        _It begin() { return { this }; }

        Empty end() { return Empty{}; }

    private:
        _Node *m_head, *m_tail;
        usize  m_count;
    };

    template <typename T>
    class LinkedList<T>::_Node
    {
        _Node *m_previous, *m_next;
        T      m_value;
    };

    template <typename T>
    class LinkedList<T>::_It
    {
    private:
        LinkedList<T>*        m_list;
        LinkedList<T>::_Node* m_current;

    public:
        T& operator*() { return m_current->m_value; }

        bool operator!=(Empty) { return m_current != nullptr; }

        _It& operator++()
        {
            if (m_current != nullptr) {
                m_current = m_current->m_next;
            }
            return *this;
        }
    };
}