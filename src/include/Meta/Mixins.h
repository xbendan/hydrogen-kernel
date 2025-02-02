namespace Meta {
    template <typename... Ts>
    struct Mixin : Ts...
    {
        using Ts::operator()...;
    };
}