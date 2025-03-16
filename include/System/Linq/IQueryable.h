#pragma once

#include <System/Linq/IEnumerator.h>

namespace System::Linq {
    template <typename T, typename Inner = T::InnerType>
    concept IQueryable = requires(T& t) {
        typename T::ExprSet;
        typename T::ResultType;

        IEnumerator<T, Inner>;
    };
}