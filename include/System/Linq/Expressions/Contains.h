#pragma once

#include <Concepts.h>
#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerator.h>

namespace System::Linq::Expressions {
    template <typename TExpr, typename Inner = typename TExpr::InnerType>
    struct ContainsExpr
    {
        using ResultType = bool;

        always_inline constexpr ResultType operator()()
        {
            for (auto it = _expr; it != Empty{}; ++it) {
                if (_value == *it) {
                    return true;
                }
            }

            return false;
        }

        TExpr        _expr;
        Inner const& _value;
    };

    template <typename T>
    constexpr auto Contains(T const& val)
    {
        return [&](auto&& en) {
            using EnumType = Std::RemoveCvRef<decltype(en)>;
            using Expr     = ContainsExpr<typename EnumType::ExprType>;
            return Enumerator<typename EnumType::InnerType, Expr>{ Expr{
                en.GetExpressions(), Std::forward<T>(val) } };
        };
    }
}