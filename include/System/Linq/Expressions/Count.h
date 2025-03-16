#pragma once

#include <Concepts.h>
#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerator.h>

namespace System::Linq::Expressions {
    template <typename TExpr>
    struct CountExpr
    {
        using ResultType = usize;
        using ExprSet    = TExpr;

        always_inline constexpr ResultType operator()()
        {
            usize count = 0;
            for (auto it = _expr; it != Empty{}; ++it)
                count += 1;

            return count;
        }

        TExpr _expr;
    };

    constexpr auto Count()
    {
        return [&](auto&& en) {
            using EnumType = Std::RemoveCvRef<decltype(en)>;
            using Expr     = CountExpr<typename EnumType::ExprSet>;
            return Enumerator<typename EnumType::ResultType, Expr>{ Expr{
                en.Expr } };
        };
    }
}

#define Count$ (Count())