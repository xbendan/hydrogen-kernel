#include <Concepts.h>
#include <Types.h>

namespace System::Hal {
    template <u16 TNum = 0>
    class PortAccess
    {
    public:
        template <class T = u8>
            requires Std::isIntegral<T>
        T in()
        {
            T data;
            asm volatile("in %1, %0" : "=a"(data) : "dN"(TNum));
            return data;
        }

        template <typename T = u8>
            requires Std::isIntegral<T>
        T out(T data)
        {
            asm volatile("out %1, %0" : : "d"(TNum), "a"(data));
            return data;
        }

        template <typename T = u8>
            requires Std::isIntegral<T>
        void operator<<(T data)
        {
            out(data);
        }

        template <typename T = u8>
            requires Std::isIntegral<T>
        void operator>>(T& data)
        {
            data = in<T>();
        }

        template <typename T = u8>
            requires Std::isIntegral<T>
        T operator()()
        {
            return in<T>();
        }

        template <typename T = u8>
            requires Std::isIntegral<T>
        void operator()(T data)
        {
            out(data);
        }

        template <typename T>
            requires Std::isIntegral<T>
        static inline void Out(u16 port, T data)
        {
            asm volatile("out %0, %1" : : "a"(data), "d"(port));
        }

        template <typename T>
        static inline T In(u16 port)
        {
            T data;
            asm volatile("in %1, %0" : "=a"(data) : "d"(port));
            return data;
        }
    };

    template <typename TSource, usize TNum, typename TAccess = TSource>
        requires Std::isIntegral<TSource>
    void operator<<(TSource& input, PortAccess<TNum>& port)
    {
        input = port.template in<TAccess>();
    }

    template <typename T = u8>
        requires Std::isIntegral<T>
    static inline T in(u16 port)
    {
        T data;
        asm volatile("in %1, %0" : "=a"(data) : "d"(port));
        return data;
    }

    template <typename T = u8>
        requires Std::isIntegral<T>
    static inline void in(u16 port, T* data, usize len)
    {
        for (usize i = 0; i < len; i++) {
            data[i] = in<T>(port);
        }
    }

    template <typename T = u8>
        requires Std::isIntegral<T>
    static inline void out(u16 port, T data)
    {
        asm volatile("out %0, %1" : : "a"(data), "d"(port));
    }

    template <typename T = u8>
        requires Std::isIntegral<T>
    static inline void out(u16 port, T* data, usize len)
    {
        for (usize i = 0; i < len; i++) {
            out<T>(port, data[i]);
        }
    }

    template <typename T = u8, bool Eq = true>
        requires Std::isIntegral<T>
    static inline void wait(u16 port, u8 bit)
    {
        T  data;
        u8 mask;
        if constexpr (Eq) {
            mask = bit;
        } else {
            mask = 0;
        }

        do {
            data = in<T>(port);
        } while ((data & bit) == mask);
    }

    static inline void waitIO()
    {
        out<u8>(0x80, 0);
    }
}