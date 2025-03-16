#include <Utility.h>

namespace Std {
    // MARK: - Invoke

    namespace _ {
        template <typename T>
        struct InvokeImpl
        {
            template <typename Fn, typename... Args>
            static auto call(Fn&& fn, Args&&... args)
                -> decltype(Std::forward<Fn>(fn)(Std::forward<Args>(args)...));
        };

        template <typename B, typename MT>
        struct InvokeImpl<B MT::*>
        {
            template <typename T, typename Td = Decay<T>::Type>
                requires isChildTypeOf<B, Td>
            static auto get(T&& t) -> T&&;

            template <typename T, typename Td = Decay<T>::Type>
                requires isReference<Td>
            static auto get(T&& t) -> decltype(t.get());
        };
    }
}