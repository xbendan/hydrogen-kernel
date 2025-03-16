#pragma once

#include <Concepts.h>
#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerator.h>

namespace System::Linq::Expressions {
    template <IEnumerator TExpr, Invokable<typename TExpr::ResultType> Fn>
        requires Same<InvokeResult<Fn, typename TExpr::ResultType>, bool>
    struct WhereExpr
    {
        using InnerType  = typename TExpr::ResultType;
        using ResultType = InnerType;
        using ExprType   = TExpr;

        TExpr _expr;
        Fn    _fn;

        always_inline InnerType operator*() { return *_expr; }

        always_inline WhereExpr& operator++()
        {
            do {
                ++_expr;
            } while (_expr != Empty{} && !_fn(*_expr));

            return *this;
        }

        always_inline bool operator!=(Empty) { return _expr != Empty{}; }

        always_inline void operator=(Empty) { _expr = Empty{}; }
    };

    template <typename Fn>
    constexpr auto Where(Fn&& fn)
    {
        return [&](auto&& en) {
            using EnumType = Std::RemoveCvRef<decltype(en)>;
            using Expr     = WhereExpr<typename EnumType::ExprSet, Fn>;
            return Enumerator<typename EnumType::ResultType, Expr>{ Expr{
                en.Expr, Std::forward<Fn>(fn) } };
        };
    }
}

#define Where$(EXPR) (Where([](auto it) { return EXPR; }))