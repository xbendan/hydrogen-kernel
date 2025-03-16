#pragma once

#include <TypeTraits.h>
#include <Utility.h>

// template <typename Fn, typename... Args>
// concept Invocable = requires(Fn fn, Args... args) {
//     { fn(declval<Args>()...) };
// };

// template <typename T, typename... Args>
// using InvokeResult =
//     decltype(declval<T>()(Std::forward<Args>(declval<Args>())...));

template <typename T0, typename T1>
concept ChildTypeOf = (Std::isChildTypeOf<T0, T1>);

template <typename T, typename... Ts>
concept Included = (Std::isSame<T, Ts> || ...);

template <typename T0, typename T1>
concept Same = Std::isSame<T0, T1>;

template <typename T0, typename T1>
concept ConvertibleTo = Std::isConvertible<T0, T1>;

template <typename T>
concept Equatable = requires(T t) {
    { t == t } -> Same<bool>;
    { t != t } -> Same<bool>;
};

template <typename T>
concept Comparable = requires(T t) {
    { t < t } -> Same<bool>;
    { t > t } -> Same<bool>;
    { t <= t } -> Same<bool>;
    { t >= t } -> Same<bool>;
};

template <typename T>
concept Computable = requires(T t) {
    { t + t } -> Same<T>;
    { t - t } -> Same<T>;
    { t* t } -> Same<T>;
    { t / t } -> Same<T>;
};

template <typename T, typename T1, typename T2>
concept Either = (Same<T, T1> || Same<T, T2>);

template <typename T, typename T1>
concept Maybe = Either<T, T1, Empty>;

namespace Std {
    namespace _ {
        template <class C, class Pt, class Obj, class... Args>
        constexpr decltype(auto)
        _InvokeMemptr(Pt C::*f, Obj&& object, Args&&... args)
        {
            using ObjectType                = RemoveCvRef<Obj>;
            constexpr bool isMemberFunction = isFunction<Pt>;
            constexpr bool isDerivedObject =
                isSame<C, ObjectType> || isChildTypeOf<C, ObjectType>;

            if constexpr (isMemberFunction) {
                if constexpr (isDerivedObject)
                    return (Std::forward<Obj>(object).*f)(
                        Std::forward<Args>(args)...);
                else
                    return ((*Std::forward<Obj>(object)).*f)(
                        Std::forward<Args>(args)...);
            } else {
                static_assert(
                    isObject<Pt> && sizeof...(Args) == 0,
                    "Pointer to member object must be a pointer to "
                    "member function");
                if constexpr (isDerivedObject)
                    return Std::forward<Obj>(object).*f;
                else
                    return (*Std::forward<Obj>(object)).*f;
            }
        }

        template <class T>
        struct _InvokeImpl
        {
            template <class F, class... Args>
            static auto Call(F&& f, Args&&... args)
                -> decltype(Std::forward<F>(f)(Std::forward<Args>(args)...));
        };

        template <class B, class MT>
        struct _InvokeImpl<MT B::*>
        {
            template <class T, class Td = typename Decay<T>::Type>
                requires isChildTypeOf<B, Td>
            static auto Get(T&& t) -> T&&;

            template <class T, class Td = typename Decay<T>::Type>
                requires(!isChildTypeOf<B, Td>)
            static auto Get(T&& t) -> decltype(*Std::forward<T>(t));

            template <class T, class... Args, class MT1>
                requires isFunction<MT1>
            static auto Call(MT1 B::*f, T&& obj, Args&&... args)
                -> decltype((_InvokeImpl::Get(Std::forward<T>(obj)).*f)(
                    Std::forward<Args>(args)...));

            template <class T>
            static auto Call(MT B::*f, T&& obj)
                -> decltype(_InvokeImpl::Get(Std::forward<T>(obj)).*f);
        };

        template <class F, class... Args, class Fd = typename Decay<F>::Type>
        auto _Invoke(F&& f, Args&&... args) -> decltype(_InvokeImpl<Fd>::Call(
                                                Std::forward<F>(f),
                                                Std::forward<Args>(args)...));

        template <typename AlwaysVoid, typename, typename...>
        struct _InvokeResult
        {};

        template <typename F, typename... Args>
        struct _InvokeResult<
            decltype(void(_Invoke(declval<F>(), declval<Args>()...))),
            F,
            Args...>
        {
            using Type = decltype(_Invoke(declval<F>(), declval<Args>()...));
        };
    }

    // template <typename Fn, class... Args>
    // concept Callable = requires(Fn&& f, Args&&... args) {

    // };

    template <typename Fn, typename... Args>
    concept Invokable = requires(Fn&& f, Args&&... args) {
        { _::_Invoke(Std::forward<Fn>(f), Std::forward<Args>(args)...) };
    };

    template <typename Fn, typename... Args>
    using InvokeResult = typename _::_InvokeResult<void, Fn, Args...>::Type;
}

using Std::Invokable;
using Std::InvokeResult;