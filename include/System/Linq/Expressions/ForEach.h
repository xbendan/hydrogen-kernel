#pragma once

#include <Concepts.h>
#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerator.h>

namespace System::Linq::Expressions {
    template <IEnumerator TExpr, Invokable<typename TExpr::ResultType> Fn>
    struct ForEachExpr
    {
        using ResultType = void;

        always_inline void operator()()
        {
            for (auto it = _expr; it != Empty{}; ++it)
                _fn(*it);
        }

        TExpr _expr;
        Fn    _fn;
    };

    template <typename Fn>
    constexpr auto ForEach(Fn&& fn)
    {
        return [&](auto&& en) {
            using EnumType = Std::RemoveCvRef<decltype(en)>;
            using Expr     = ForEachExpr<typename EnumType::ExprSet, Fn>;
            return Enumerator<typename EnumType::ResultType, Expr>{ Expr{
                en.Expr, Std::forward<Fn>(fn) } };
        };
    }
}

#define ForEach$(EXPR) (Select([](auto it) { EXPR; }))
