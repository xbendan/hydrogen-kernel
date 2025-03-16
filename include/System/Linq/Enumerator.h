#pragma once

#include <TypeTraits.h>

#include <System/Linq/IAggregator.h>
#include <System/Linq/IEnumerator.h>
#include <System/Linq/IQueryable.h>

#include <Decl.h>

namespace System::Linq {
    template <typename TResult, typename TExpressions>
    struct Enumerator
    {
        using InnerType  = TResult;
        using ResultType = TResult;
        using ExprSet    = TExpressions;

        always_inline constexpr ResultType operator*() const { return *Expr; }

        always_inline constexpr Enumerator& operator++()
        {
            ++Expr;
            return *this;
        }

        always_inline constexpr bool operator!=(Empty) const
        {
            return Expr != Empty{};
        }

        always_inline constexpr void operator=(Empty) { Expr = Empty{}; }

        TExpressions Expr;
    };

    template <IEnumerator TEnumerator, typename TBuilder>
    constexpr decltype(auto) operator|(
        TEnumerator&& enumerator,
        TBuilder&&    builder)
    {
        return Std::forward<TBuilder>(builder)(
            Std::forward<TEnumerator>(enumerator));
    }

    template <IEnumerator TEnumerator, typename TBuilder>
    constexpr auto operator>>(TEnumerator&& enumerator, TBuilder&& builder)
    {
        auto var = Std::forward<TBuilder>(builder)(
            Std::forward<TEnumerator>(enumerator));

        return var.Expr();
    }
}