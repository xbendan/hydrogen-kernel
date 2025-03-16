#pragma once

#include <Concepts.h>
#include <TypeInfo.h>
#include <Types.h>
#include <Utility.h>

namespace Meta {
    namespace _ {
        always_inline constexpr auto max(auto value)
        {
            return value;
        }

        always_inline constexpr auto max(auto first, auto... rest)
        {
            auto rhs = max(rest...);
            return first > rhs ? first : rhs;
        }
    }

    template <typename... Ts>
    class Union
    {
    public:
        always_inline Union() = delete;

        template <Included<Ts...> T>
        always_inline Union(T const& value)
            : _index(indexOf<T, Ts...>())
        {
            new (_data) T(value);
        }

        template <Included<Ts...> T>
        always_inline Union(T&& value)
            : _index(indexOf<T, Ts...>())
        {
            new (_data) T(Std::move(value));
        }

        always_inline Union(Union<Ts...> const& other)
            : _index(other._index)
        {
            indexCast<Ts...>(
                _index, other._data, [this]<typename T>(T const& value) {
                    new (_data) T(value);
                });
        }

        always_inline Union(Union<Ts...>&& other)
            : _index(other._index)
        {
            indexCast<Ts...>(
                _index, other._data, [this]<typename T>(T&& value) {
                    new (_data) T(Std::move(value));
                });
        }

        always_inline ~Union()
        {
            indexCast<Ts...>(
                _index, _data, []<typename T>(T& value) { value.~T(); });
        }

        template <Included<Ts...> T>
        always_inline Union& operator=(T const& value)
        {
            indexCast<Ts...>(
                _index, _data, [this]<typename U>(U& value) { value.~U(); });

            _index = indexOf<T, Ts...>();
            new (_data) T(value);
            return *this;
        }

        template <Included<Ts...> T>
        always_inline Union& operator=(T&& value)
        {
            indexCast<Ts...>(
                _index, _data, [this]<typename U>(U& value) { value.~U(); });

            _index = indexOf<T, Ts...>();
            new (_data) T(Std::move(value));
            return *this;
        }

        always_inline Union& operator=(Union<Ts...> const& other)
        {
            indexCast<Ts...>(
                _index, _data, [this]<typename T>(T& value) { value.~T(); });

            _index = other._index;
            indexCast<Ts...>(
                _index, other._data, [this]<typename T>(T const& value) {
                    new (_data) T(value);
                });
            return *this;
        }

        always_inline Union& operator=(Union<Ts...>&& other)
        {
            indexCast<Ts...>(
                _index, _data, [this]<typename T>(T& value) { value.~T(); });

            _index = other._index;
            indexCast<Ts...>(
                _index, other._data, [this]<typename T>(T&& value) {
                    new (_data) T(Std::move(value));
                });
            return *this;
        }

        template <Included<Ts...> T>
        always_inline T& unwrap()
        {
            if (_index != indexOf<T, Ts...>()) [[unlikely]] {
                // Std::terminate();
            }

            return *reinterpret_cast<T*>(_data);
        }

        template <Included<Ts...> T>
        always_inline T const& unwrap() const
        {
            if (_index != indexOf<T, Ts...>()) [[unlikely]] {
                // Std::terminate();
            }

            return *reinterpret_cast<T const*>(_data);
        }

        template <Included<Ts...> T>
        always_inline T const* is() const
        {
            return _index == indexOf<T, Ts...>()
                       ? reinterpret_cast<T const*>(_data)
                       : nullptr;
        }

        template <Included<Ts...> T>
        always_inline T* is()
        {
            return _index == indexOf<T, Ts...>() ? reinterpret_cast<T*>(_data)
                                                 : nullptr;
        }

        always_inline u8 index() const { return _index; }

    private:
        alignas(_::max(alignof(Ts)...)) byte _data[_::max(sizeof(Ts)...)];
        u8 _index;
    };
}

using Meta::Union;