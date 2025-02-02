#include <StdC++/Types.h>
#include <StdC++/Utility.h>

namespace Meta {
    template <typename T>
    struct Manual
    {
        using InnerType = T;

        alignas(alignof(T)) byte _data[sizeof(T)];

        template <typename... Args>
        always_inline T* operator()(Args&&... args)
        {
            return new (&get()) T{ Std::forward<Args>(args)... };
        }

        always_inline void destroy() { get().~T(); }

        always_inline T& get() { return *(T*)_data; }

        always_inline T const& get() const { return *(T const*)_data; }

        always_inline T* operator->() { return &get(); }

        always_inline T const* operator->() const { return &get(); }

        always_inline T& operator*() { return get(); }

        always_inline T const& operator*() const { return get(); }

        always_inline operator T&() { return get(); }

        always_inline operator T const&() const { return get(); }

        always_inline T take()
        {
            T result = get();
            destroy();
            return result;
        }
    };
}

using Meta::Manual;