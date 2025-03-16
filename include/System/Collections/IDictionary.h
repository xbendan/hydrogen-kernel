#pragma once

#include <Meta/Optional.h>
#include <Meta/Pair.h>
#include <System/Collections/ICollection.h>
#include <System/Linq/IEnumerator.h>

namespace System::Collections {
    using Linq::IEnumerator;

    template <
        typename T,
        typename K = typename T::KeyType,
        typename V = typename T::ValueType>
    struct DictionaryAccess;

    template <
        typename T,
        typename K     = typename T::Key,
        typename V     = typename T::Value,
        typename Inner = Pair<K const&, V const&>>
    concept IDictionary =
        requires(T& t, K const& key, V const& val, Inner pair) //
    {
        ICollection<T, Inner>;

        { t[key] } -> Same<DictionaryAccess<T, K, V>>;
        { t += pair };                  // Equivalent to Insert
        { t -= key };                   // Equivalent to Remove
        { t -= pair };                  // Equivalent to Remove (Value Matched)
        { t != Empty{} } -> Same<bool>; // Equivalent to not IsEmpty
        { t == Empty{} } -> Same<bool>; // Equivalent to IsEmpty
        { t = Empty{} };                // Equivalent to Clear

        { t.Insert(key, val) } -> Same<V const&>;
        { t.InsertIfAbsent(key, val) } -> Same<bool>;
        { t.Replace(key, val) } -> Same<bool>;
        { t.Replace(key, val, val) } -> Same<bool>;
        { t.Remove(key) } -> Same<bool>;
        { t.Remove(key, val) } -> Same<bool>;
        { t.Contains(key) } -> Same<bool>;
        { t.Contains(Inner(key, val)) } -> Same<bool>;
        { t.Count() } -> Same<usize>;
        { t.Keys() } -> IEnumerator<K const&>;
        { t.Values() } -> IEnumerator<V&>;
    };

    template <typename T, typename K, typename V>
        requires IDictionary<T, K, V>
    struct DictionaryAccess<T, K, V>
    {
        T&           _dictionary;
        K const&     _key;
        Optional<V&> _value;

        always_inline V const& operator=(V const& value)
        {
            return _dictionary.Put(_key, value);
        }

        always_inline void operator=(Empty) { _dictionary.Remove(_key); }

        always_inline operator V const&() { return _value.value(); }

        always_inline operator bool() { return _value.isPresent(); }

        always_inline bool operator==(Empty) { return !_value.isPresent(); }

        always_inline bool operator!=(Empty) { return _value.isPresent(); }

        always_inline V const& operator*() { return _value.value(); }
    };
}