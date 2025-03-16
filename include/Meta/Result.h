#pragma once

#include <Meta/Indexer.h>
#include <Meta/Union.h>
#include <Types.h>
#include <Utility.h>

namespace Meta {
    // MARK: - [Ok]

    template <typename T = Empty>
    struct Ok
    {
        T _inner;

        template <typename... Args>
        always_inline constexpr Ok(Args&&... args)
            : _inner{ Std::forward<Args>(args)... }
        {
        }

        T take() { return Std::move(_inner); }
    };

    template <typename... Args>
    Ok(Args&&...) -> Ok<Std::RemoveCvRef<Args>...>;

    // MARK: - [Result]

    template <typename V = Empty, typename... Errs>
    struct Result
    {
        using InnerType = Union<Ok<V>, Errs...>;

        InnerType _inner;

        always_inline constexpr Result(Ok<V> const& ok)
            : _inner{ ok }
        {
        }

        always_inline constexpr Result(Ok<V>&& ok)
            : _inner{ Std::move(ok) }
        {
        }

        template <Included<Errs...> E>
        always_inline constexpr Result(E const& err)
            : _inner{ err }
        {
        }

        template <typename U>
        always_inline constexpr Result(Result<U, Errs...> const& other)
            : _inner{ other._inner }
        {
        }

        always_inline constexpr ~Result() = default;

        always_inline constexpr explicit operator bool() const
        {
            return _inner.template is<Ok<V>>();
        }

        always_inline constexpr bool isSuccess() const
        {
            return _inner.template is<Ok<V>>();
        }

        always_inline constexpr bool isError() const { return not isSuccess(); }

        always_inline constexpr V& get(
            char const* message =
                "Result::get() called on an error value") & noexcept
        {
            if (not _inner.template is<Ok<V>>()) {
                // panic
            }

            return _inner.template get<Ok<V>>()._inner;
        }

        always_inline constexpr V const& get(
            char const* message =
                "Result::get() called on an error value") const& noexcept
        {
            if (not _inner.template is<Ok<V>>()) {
                // panic
            }

            return _inner.template get<Ok<V>>()._inner;
        }

        template <Included<Errs...> E, typename Fn>
            requires Invokable<Fn, E const&>
        always_inline constexpr void catchErr(Fn&& fn) const
        {
            if (_inner.template is<E>()) {
                fn(_inner.template get<E>());
            }
        }

        template <typename T, typename Fn>
            requires Invokable<Fn, V const&>
        always_inline constexpr Result<T, Errs...> map(Fn&& fn) const
        {
            if (_inner.template is<Ok<V>>()) {
                return Ok<T>{ fn(_inner.template get<Ok<V>>()._inner) };
            }

            return *this;
        }
    };
}

using Meta::Ok;
using Meta::Result;