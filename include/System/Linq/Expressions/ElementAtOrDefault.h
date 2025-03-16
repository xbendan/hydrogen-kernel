#pragma once

#include <Concepts.h>
#include <Meta/Optional.h>
#include <System/Linq/Enumerator.h>
#include <System/Linq/IEnumerator.h>

namespace System::Linq::Expressions {
    template <typename TExpr, typename Inner = typename TExpr::InnerType>
    struct ElementAtOrDefaultExpr
    {
        always_inline constexpr explicit operator Inner&()
        {
            usize count = 0;
            for (auto it = _expr; it != Empty{}; ++it) {
                if (count == _index)
                    return *it;

                count += 1;
            }

            return _defaultValue;
        }

        always_inline constexpr Optional<Inner> operator->()
        {
            usize count = 0;
            for (auto it = _expr; it != Empty{}; ++it) {
                if (count == _index)
                    return *it;

                count += 1;
            }

            return _defaultValue;
        }

        TExpr        _expr;
        usize        _index;
        Inner const& _defaultValue;
    };

    template <typename TExpr, typename Inner = typename TExpr::InnerType>
    constexpr auto ElementAt(usize index, Inner const& defaultValue)
    {
        return [index, &defaultValue](auto&& en) {
            using EnumType = Std::RemoveCvRef<decltype(en)>;
            using Expr = ElementAtOrDefaultExpr<typename EnumType::ExprType>;
            return Enumerator<typename EnumType::InnerType, Expr>{ Expr{
                en.GetExpressions(), index, defaultValue } };
        };
    }
}