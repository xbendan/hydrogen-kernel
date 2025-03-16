#pragma once

#include <Concepts.h>
#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerator.h>

namespace System::Linq::Expressions {
    template <typename TExpr, Invokable<typename TExpr::InnerType> Fn>
        requires Same<InvokeResult<Fn, typename TExpr::InnerType>, bool>
    struct AllExpr
    {
        always_inline constexpr explicit operator bool()
        {
            for (auto it = _expr; it != Empty{}; ++it) {
                if (!_fn(*it)) {
                    return false;
                }
            }

            return true;
        }

        TExpr _expr;
        Fn    _fn;
    };

    template <typename Fn>
    constexpr auto All(Fn&& fn)
    {
        return [&](auto&& en) {
            using EnumType = Std::RemoveCvRef<decltype(en)>;
            using Expr     = AllExpr<typename EnumType::ExprType, Fn>;
            return Enumerator<typename EnumType::ResultType, Expr>{ Expr{
                en.GetExpressions(), Std::forward<Fn>(fn) } };
        };
    }
}

#define All$(EXPR) (All([](auto it) { return EXPR; }))