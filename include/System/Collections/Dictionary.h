#pragma once

#include <Meta/Array.h>
#include <System/Collections/Generic/Hash.h>
#include <System/Collections/Generic/IComparator.h>
#include <System/Collections/Generic/IHash.h>
#include <System/Collections/IDictionary.h>
#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerator.h>
#include <System/Security/Cryptography/Hash.h>
#include <Types.h>

namespace System::Collections {
    using Crypto::GetPrime;
    using Generic::Hash;
    using Generic::IHasher;
    using Linq::Enumerator;
    using Linq::IEnumerator;

    // MARK: - [Dictionary]

    template <typename TKey, typename TValue>
    class Dictionary
    {
        struct Entry
        {
            i32    hashCode; // Lower 31 bits of hash code, -1 if unused
            i32    next;     // Index of next entry, -1 if last
            TKey   key;
            TValue value;
        };

    public:
        static constexpr u32 sc_HashCollisionThreshold = 8;

        using KeyType      = TKey;
        using ValueType    = TValue;
        using KeyValuePair = Pair<TKey const&, TValue const&>;

        struct KeyIt;
        struct ValueIt;
        struct EntryIt;

        Dictionary(usize capacity)
            : m_buckets(GetPrime(capacity))
            , m_entries(GetPrime(capacity))
            , m_count(0)
            , m_version(0)
            , m_freeindex(-1)
        {
            Arrays::Fill(m_buckets, -1);
        }

        Dictionary(IDictionary<TKey, TValue> auto& other)
            : Dictionary(other.Count())
        {
            for (auto& [key, value] : other) {
                Insert(key, value);
            }
        }

        // MARK: - Resize

        void Resize(i32 newSize)
        {
            assert(
                newSize >= m_entries.Length(),
                "Dictionary: New size must be larger than current size");
            Array<i32>    newBuckets(newSize);
            Array<Entry*> newEntries(newSize);
            Arrays::Fill(newBuckets, -1);

            Arrays::Copy(*m_entries, 0, *newEntries, 0, m_count);
            for (i32 i = 0; i < m_count; i++) {
                if (newEntries[i]->hashCode >= 0) {
                    u32 bucket          = newEntries[i]->hashCode % newSize;
                    newEntries[i]->next = newBuckets[bucket];
                    newBuckets[bucket]  = i;
                }
            }
            m_buckets = newBuckets;
            m_entries = newEntries;
        }

        // MARK: - FindEntry

        i32 FindEntry(TKey const& key)
        {
            u32 hashval = m_hash(key) & 0x7FFFFFFF;
            u32 bucket  = hashval % m_buckets.Length();

            for (u32 i = m_buckets[bucket]; // Start from the bucket
                 i != -1;
                 i = m_entries[i]->next) {

                if (m_entries[i]->hashCode == hashval &&
                    m_entries[i]->key == key) {
                    return i;
                }
            }
            return -1;
        }

        // MARK: - Insert

        /**
         * @brief Put a key-value pair into the dictionary.
         *
         * @param key
         * @param value
         * @return TValue& The reference to stored value.
         */
        TValue& Insert(TKey const& key, TValue const& value)
        {
            i32 hashval = m_hash(key) & 0x7FFFFFFF;
            u32 bucket  = hashval % m_buckets.Length();

            u32 collisions = 0;
            for (i32 i = m_buckets[bucket]; // Start from the bucket
                 i != -1;
                 i = m_entries[i]->next) {

                if (m_entries[i]->hashCode == hashval &&
                    m_entries[i]->key == key) {
                    m_entries[i]->value = value;
                    m_version++;

                    return m_entries[i]->value;
                }
                collisions++;
            }

            i32 index;
            if (m_freeindex != -1) {
                index       = m_freeindex;
                m_freeindex = m_entries[m_freeindex]->next;
            } else {
                if (m_count == m_entries.Length()) {
                    Resize(m_count * 2);
                    bucket = hashval % m_buckets.Length();
                }
                index = m_count;
                m_count++;
            }

            *m_entries[index] = { hashval, m_buckets[bucket], key, value };
            m_buckets[bucket] = index;
            m_version++;

            if (collisions > sc_HashCollisionThreshold) {
                Resize(m_buckets.Length() * 2);
            }

            return m_entries[index]->value;
        }

        // MARK: - InsertIfAbsent

        /**
         * @brief Put a key-value pair into the dictionary if the key does
         * not exist.
         *
         * @param key
         * @param value
         * @return TValue& The reference to current stored value.
         */
        TValue& InsertIfAbsent(TKey const& key, TValue const& value);

        // MARK: - Replace

        /**
         * @brief Replace the value of a key.
         *
         * @param key
         * @param value
         * @return true If the key exists.
         * @return false If the key does not exist.
         */
        bool Replace(TKey const& key, TValue const& value)
        {
            i32 index = FindEntry(key);
            if (index == -1) {
                return false;
            }

            m_entries[index]->value = value;
            m_version++;

            return true;
        }

        // MARK: - Replace (Value Matched)

        /**
         * @brief Replace the value of a key if the old value matches.
         *
         * @param key
         * @param oldValue
         * @param newValue
         * @return true
         * @return false
         */
        bool
        Replace(TKey const& key, TValue const& oldValue, TValue const& newValue)
        {
            i32 index = FindEntry(key);
            if (index == -1 || m_entries[index]->value != oldValue) {
                return false;
            }

            m_entries[index]->value = newValue;
            m_version++;

            return true;
        }

        // MARK: - Remove

        /**
         * @brief Remove the entry with the given key.
         *
         * @param key
         * @return true If the key exists.
         * @return false If the key does not exist.
         */
        bool Remove(TKey const& key)
        {
            u32 hashval = m_hash(key) & 0x7FFFFFFF;
            u32 bucket  = hashval % m_buckets.Length();
            i32 last    = -1;

            for (i32 i = m_buckets[bucket]; // Start from the bucket
                 i != -1;
                 i = m_entries[i]->next) {

                if (m_entries[i]->hashCode == hashval &&
                    m_entries[i]->key == key) {
                    if (last == -1) {
                        m_buckets[bucket] = m_entries[i]->next;
                    } else {
                        m_entries[last]->next = m_entries[i]->next;
                    }

                    m_entries[i]->hashCode = -1;
                    m_entries[i]->next     = m_freeindex;
                    m_freeindex            = i;
                    m_version++;
                    m_freecount++;

                    return true;
                }
                last = i;
            }

            return false;
        }

        // MARK: - Remove (Value Matched)

        /**
         * @brief Remove the entry with the given key and value.
         *
         * @param key
         * @param value
         * @return true If the key exists and the value matches.
         * @return false If the key does not exist or the value does not
         * match.
         */
        bool Remove(TKey const& key, TValue const& value)
        {
            u32 hashval = m_hash(key) & 0x7FFFFFFF;
            u32 bucket  = hashval % m_buckets.Length();
            u32 last    = -1;

            for (i32 i = m_buckets[bucket]; // Start from the bucket
                 i != -1;
                 i = m_entries[i]->next) {

                if (m_entries[i]->hashCode == hashval &&
                    m_entries[i]->key == key && m_entries[i]->value == value) {
                    if (last == -1) {
                        m_buckets[bucket] = m_entries[i]->next;
                    } else {
                        m_entries[last]->next = m_entries[i]->next;
                    }

                    m_entries[i]->hashCode = -1;
                    m_entries[i]->next     = m_freeindex;
                    m_freeindex            = i;
                    m_version++;
                    m_freecount++;

                    return true;
                }
                last = i;
            }

            return false;
        }

        // MARK: - Contains

        /**
         * @brief Check if the dictionary contains the given key.
         *
         * @param key
         * @return true If the key exists.
         * @return false If the key does not exist.
         */
        bool Contains(TKey const& key) { return FindEntry(key) != -1; }

        // MARK: - Contains (KeyValuePair)

        /**
         * @brief Check if the dictionary contains the given key-value pair.
         *
         * @param pair
         * @return true If the key-value pair exists.
         * @return false If the key-value pair does not exist.
         */
        bool Contains(KeyValuePair pair)
        {
            i32 index = FindEntry(pair.first);
            return index != -1 && m_entries[index]->value == pair.second;
        }

        // MARK: - Count

        usize Count() const { return m_count - m_freecount; }

        // MARK: - Dict[Key]

        DictionaryAccess<Dictionary<TKey, TValue>> operator[](TKey const& key)
        {
            i32 index = FindEntry(key);
            if (index == -1) {
                return { *this, key, Empty() };
            }

            return { *this, key, m_entries[index]->value };
        }

        // MARK: - Dict += Pair

        void operator+=(KeyValuePair pair) { Insert(pair.first, pair.second); }

        // MARK: - Dict -= Key

        void operator-=(TKey const& key) { Remove(key); }

        // MARK: - Dict -= Pair

        void operator-=(KeyValuePair pair) { Remove(pair.first, pair.second); }

        // MARK: - Dict != Empty

        explicit operator bool() const { return m_count != 0; }

        // MARK: - Dict == Empty

        bool operator==(Empty) const { return m_count == 0; }

        // MARK: - Dict = Empty

        void operator=(Empty)
        {
            for (auto* entry : m_entries) {
                if (entry)
                    delete entry;
            }
            Arrays::Fill(m_entries, nullptr);
            Arrays::Fill<i32>(m_buckets, -1);
            m_count     = 0;
            m_version   = 0;
            m_freeindex = -1;
        }

        Enumerator<TKey, KeyIt>     Keys() { return { KeyIt{ *this } }; }
        Enumerator<TValue, ValueIt> Values() { return { ValueIt{ *this } }; }
        Enumerator<KeyValuePair, EntryIt> Entries()
        {
            return { EntryIt{ *this } };
        }

    private:
        Array<i32>    m_buckets;
        Array<Entry*> m_entries;
        usize         m_count;
        int           m_version; // For enumerator to detect changes
        i32           m_freeindex{ -1 };
        u32           m_freecount{ 0 };
        Hash<TKey>    m_hash;
    };

    // MARK: - Keys

    template <typename TKey, typename TValue>
    struct Dictionary<TKey, TValue>::KeyIt
    {
        using InnerType = TKey;

        Dictionary<TKey, TValue>& dict;
        usize                     index;
        int                       version;

        KeyIt(Dictionary<TKey, TValue>& dict, usize index = 0)
            : dict(dict)
            , index(index)
            , version(dict.m_version)
        {
        }

        TKey& operator*() const
        {
            assert(
                version == dict.m_version,
                "Dictionary modified during iteration");
            return dict.m_entries[index]->key;
        }

        KeyIt& operator++()
        {
            assert(
                version == dict.m_version,
                "Dictionary modified during iteration");
            while (++index < dict.m_entries.Length()) {
                if (dict.m_entries[index]) {
                    break;
                }
            }

            return *this;
        }

        bool operator!=(Empty) const { return index < dict.m_entries.Length(); }

        void operator=(Empty) { index = 0; }
    };
    static_assert(IEnumerator<Dictionary<int, int>::KeyIt>);

    // MARK: - Values

    template <typename TKey, typename TValue>
    struct Dictionary<TKey, TValue>::ValueIt
    {
        using InnerType = TValue;

        Dictionary<TKey, TValue>& dict;
        usize                     index;
        int                       version;

        ValueIt(Dictionary<TKey, TValue>& dict, usize index = 0)
            : dict(dict)
            , index(index)
            , version(dict.m_version)
        {
        }

        TValue& operator*() const
        {
            assert(
                version == dict.m_version,
                "Dictionary modified during iteration");
            return dict.m_entries[index]->value;
        }

        ValueIt& operator++()
        {
            assert(
                version == dict.m_version,
                "Dictionary modified during iteration");
            while (++index < dict.m_entries.Length()) {
                if (dict.m_entries[index]) {
                    break;
                }
            }

            return *this;
        }

        bool operator!=(Empty) const { return index < dict.m_entries.Length(); }

        void operator=(Empty) { index = 0; }
    };
    static_assert(IEnumerator<Dictionary<int, int>::ValueIt>);

    // MARK: - Entries

    template <typename TKey, typename TValue>
    struct Dictionary<TKey, TValue>::EntryIt
    {
        using InnerType = KeyValuePair;

        Dictionary<TKey, TValue>& dict;
        usize                     index;
        int                       version;

        EntryIt(Dictionary<TKey, TValue>& dict, usize index = 0)
            : dict(dict)
            , index(index)
            , version(dict.m_version)
        {
        }

        InnerType operator*() const
        {
            assert(
                version == dict.m_version,
                "Dictionary modified during iteration");
            return { dict.m_entries[index]->key, dict.m_entries[index]->value };
        }

        EntryIt& operator++()
        {
            assert(
                version == dict.m_version,
                "Dictionary modified during iteration");
            while (++index < dict.m_entries.Length()) {
                if (dict.m_entries[index]) {
                    break;
                }
            }

            return *this;
        }

        bool operator!=(Empty) const { return index < dict.m_entries.Length(); }

        void operator=(Empty) { index = 0; }
    };
    static_assert(IEnumerator<Dictionary<int, int>::EntryIt>);
}