#pragma once

#include <Concepts.h>
#include <TypeTraits.h>
#include <Types.h>
#include <Utility.h>

namespace Meta {
    template <typename T>
    struct Optional;

    template <typename T>
        requires Std::isReference<T>
    struct Optional<T>
    {};

    template <typename T>
    struct Optional
    {
        union
        {
            T     _value;
            Empty _empty;
        };
        bool _present;

        always_inline constexpr Optional()
            : _empty{}
            , _present{ false }
        {
        }

        always_inline constexpr Optional(T const& value)
            : _value{ value }
            , _present{ true }
        {
        }

        always_inline constexpr Optional(Empty)
            : _empty{}
            , _present{ false }
        {
        }

        always_inline constexpr Optional(Optional const& other)
            requires Std::isCopyConstructible<T>
            : _value{ other._value }
            , _present{ other._present }
        {
        }

        always_inline constexpr Optional(Optional&& other)
            requires Std::isMoveConstructible<T>
            : _value{ other._value }
            , _present{ other._present }
        {
            other._value   = T{};
            other._present = false;
        }

        template <typename U = T>
        always_inline constexpr Optional(U&& value)
            requires(!Std::isSame<Std::RemoveCv<U>, Optional<T>> &&
                     Std::isMoveConstructible<T>)
            : _value{ Std::forward<U>(value) }
            , _present{ true }
        {
        }

        always_inline constexpr ~Optional()
        {
            if (_present)
                _value.~T();
        }

        always_inline constexpr Optional& operator=(Optional const& other)
            requires Std::isCopyConstructible<T>
        {
            _value   = other._value;
            _present = other._present;
            return *this;
        }

        always_inline constexpr Optional& operator=(Optional&& other)
            requires Std::isMoveConstructible<T>
        {
            _value         = other._value;
            _present       = other._present;
            other._value   = T{};
            other._present = false;

            return *this;
        }

        always_inline constexpr T value() const { return _value; }

        always_inline constexpr bool isPresent() const { return _present; }

        template <typename F>
            requires Invokable<F, T const&>
        always_inline constexpr void ifPresent(F f) const
        {
            if (_present)
                f(_value);
        }

        template <typename... Args>
        always_inline constexpr T& emplace(Args&&... args)
        {
            if (_present)
                _value.~T();

            _present = true;
            return new (&_value) T{ Std::forward<Args>(args)... };
        }

        always_inline constexpr T const& get(const char* message) const
        {
            if (not _present) {
                // TODO: Throw an exception
            }

            return _value;
        }

        always_inline constexpr T orElse(T other) const
        {
            if (_present)
                return _value;

            return other;
        }

        always_inline constexpr T take(const char* message = "No value present")
        {
            if (not _present) {
                // TODO: Throw an exception
            }

            T val = Std::move(_value);
            _value.~T();
            _present = false;
            return val;
        }

        template <typename T1, typename Fc>
            requires Invokable<Fc, T const&>
        always_inline constexpr Optional<T1> select(Fc f) const
        {
            if (_present)
                return f(_value);

            return Optional<T1>{};
        }

        template <typename... Args>
        always_inline constexpr auto operator()(Args&&... args) const
        {
            using R = InvokeResult<T, Args...>;

            if constexpr (Std::isSame<R, void>) {
                if (!_present)
                    return false;

                _value(Std::forward<Args>(args)...);
                return true;
            } else {
                if (!_present)
                    return Empty{};

                return Optional(_value(Std::forward<Args>(args)...));
            }
        }

        always_inline constexpr T& operator*()
        {
            if (not _present) {
                // TODO: Throw an exception
            }

            return _value;
        }

        always_inline constexpr T const& operator*() const
        {
            if (not _present) {
                // TODO: Throw an exception
            }

            return _value;
        }

        always_inline constexpr bool operator==(Empty) const
        {
            return not _present;
        }

        always_inline constexpr operator bool() const { return _present; }

        always_inline constexpr operator T() const
        {
            if (not _present) {
                // TODO: Throw an exception
            }

            return _value;
        }
    };
}

using Meta::Optional;