namespace Std {
    template <typename T, T t>
    struct Constant
    {
        static constexpr T Value = t;
        using ValueType          = T;
        using Type               = Constant<T, t>;

        constexpr operator ValueType() const noexcept { return Value; }
        constexpr ValueType operator()() const noexcept { return Value; }
    };

    template <typename TFrom, typename TTo>
    struct IsChildTypeOf : Constant<bool, __is_base_of(TFrom, TTo)>
    {};
}