#pragma once

#include <Concepts.h>
#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerator.h>

namespace System::Linq::Expressions {
    template <IEnumerator TExpr, Invokable<typename TExpr::InnerType> Fn>
    struct SelectExpr
    {
        using InnerType  = typename TExpr::ResultType;
        using ResultType = InvokeResult<Fn, InnerType>;

        always_inline ResultType operator*() { return _fn(*_expr); }

        always_inline SelectExpr& operator++()
        {
            ++_expr;
            return *this;
        }

        always_inline bool operator!=(Empty) { return _expr != Empty{}; }

        always_inline void operator=(Empty) { _expr = Empty{}; }

        TExpr _expr;
        Fn    _fn;
    };

    template <typename Fn>
    constexpr auto Select(Fn&& fn)
    {
        return [&](auto&& en) {
            using EnumType = Std::RemoveCvRef<decltype(en)>;
            using Expr     = SelectExpr<typename EnumType::ExprSet, Fn>;
            return Enumerator<typename EnumType::ResultType, Expr>{ Expr{
                en.Expr, Std::forward<Fn>(fn) } };
        };
    }
}

#define Select$(EXPR) (Select([](auto it) { return EXPR; }))
