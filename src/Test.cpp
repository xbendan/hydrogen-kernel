#include <Meta/Array.h>
#include <System/Linq/Enumerator.h>
#include <System/Linq/Expressions/Count.h>
#include <System/Linq/Expressions/ForEach.h>
#include <System/Linq/Expressions/From.h>
#include <System/Linq/Expressions/Select.h>
#include <System/Linq/Expressions/Where.h>

#include <Concepts.h>

using namespace System::Linq;
using namespace System::Linq::Expressions;

int
test_select(int i)
{
    return i * 2;
}

void
_main()
{
    Array<int, 5> arr = { 1, 2, 3, 4, 5 };
    int           x   = 0;

    auto ret = (From(arr)              //
                | Select$(it * 2)      //
                | Where$(it % 2 == 0)) //
               >> Count$;
    using T1 = decltype(ret);

    auto from = From(arr) | Select$(it * 2);
    using T2  = decltype(from);
    using EX  = T2::ExprSet::InnerType;

    From(arr)             //
        | Select$(it * 2) //
        | Where$(it % 2 == 0);

    // LinQ<>::range(0, 10).select([](auto i) { return i * 2; });
}