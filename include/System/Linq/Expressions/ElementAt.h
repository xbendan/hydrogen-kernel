#pragma once

#include <Concepts.h>
#include <Meta/Optional.h>
#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerator.h>

namespace System::Linq::Expressions {
    template <typename TExpr, typename Inner = typename TExpr::InnerType>
    struct ElementAtExpr
    {
        using ResultType = Optional<Inner>;

        always_inline constexpr ResultType operator()()
        {
            usize count = 0;
            for (auto it = _expr; it != Empty{}; ++it) {
                if (count == _index)
                    return *it;

                count += 1;
            }

            return Empty{};
        }

        TExpr _expr;
        usize _index;
    };

    constexpr auto ElementAt(usize index)
    {
        return [index](auto&& en) {
            using EnumType = Std::RemoveCvRef<decltype(en)>;
            using Expr     = ElementAtExpr<typename EnumType::ExprType>;
            return Enumerator<typename EnumType::InnerType, Expr>{ Expr{
                en.GetExpressions(), index } };
        };
    }
}