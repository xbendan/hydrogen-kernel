#pragma once

#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerator.h>
#include <TypeTraits.h>

namespace System::Linq::Expressions {
    template <typename TExpr, typename Result = typename TExpr::InnerType>
        requires Std::isArithmetic<Result>
    struct AverageExpr
    {
        using ResultType = Result;

        always_inline constexpr ResultType operator()()
        {
            ResultType sum   = 0;
            ResultType count = 0;
            for (auto it = _expr; it != Empty{}; ++it) {
                sum += *it;
                count += 1;
            }

            return sum / count;
        }

        TExpr _expr;
    };

    constexpr auto Average()
    {
        return [](auto&& en) {
            using EnumType = Std::RemoveCvRef<decltype(en)>;
            using Expr     = AverageExpr<typename EnumType::ExprType>;
            return Enumerator<typename EnumType::ResultType, Expr>{ Expr{
                en.GetExpressions() } };
        };
    }
}