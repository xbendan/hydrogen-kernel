#pragma once

#include <Concepts.h>
#include <Types.h>

namespace Meta {
    using namespace Std;

    template <typename...>
    inline constexpr usize _indexOf = 0;

    template <typename T, typename T1>
    inline constexpr usize _indexOf<T, T1> = 0;

    template <typename T, typename T1, typename... Rest>
    inline constexpr usize _indexOf<T, T1, Rest...> =
        Std::isSame<T, T1> ? 0 : _indexOf<T, Rest...> + 1;

    template <typename T, typename... Ts>
        requires Included<T, Ts...>
    static consteval usize indexOf()
    {
        return _indexOf<T, Ts...>;
    }

    template <typename...>
    struct _indexCast;

    template <typename Tdata, typename T>
    struct _indexCast<Tdata, T>
    {
        always_inline static auto eval(usize, Tdata* p, auto func)
        {
            using U = Std::Conditional<Std::isConst<Tdata>, Tdata, T>;
            return func(*reinterpret_cast<U*>(p));
        }
    };

    template <typename Tdata, typename T, typename... Ts>
    struct _indexCast<Tdata, T, Ts...>
    {
        always_inline static auto eval(usize index, Tdata* p, auto func)
        {
            if (index == 0) {
                using U = Std::Conditional<Std::isConst<Tdata>, Tdata, T>;
                return func(*reinterpret_cast<U*>(p));
            }
            return _indexCast<Tdata, Ts...>::eval(index - 1, p, func);
        }
    };

    template <typename... Ts>
    always_inline static inline auto indexCast(usize index, auto* p, auto func)
    {
        return _indexCast<typename Std::RemoveRef<decltype(*p)>, Ts...>::eval(
            index, p, func);
    }

    struct Indexer
    {
        usize index;

        constexpr Indexer(usize index)
            : index(index)
        {
        }

        constexpr operator usize() const { return index; }
    };
}

using Meta::indexCast;
using Meta::indexOf;

using Meta::Indexer;