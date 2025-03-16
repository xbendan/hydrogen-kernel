#include <Concepts.h>
#include <System/Collections/Iteration.h>
#include <System/Text/EncodingFormat.h>

namespace System::Text {
    using Collections::Iterable;
    using Collections::Iterator;

    template <typename T, typename E = typename T::Encoding>
    concept CharSequence = requires(T t) {
        EncodingFormat<E>;
        Same<typename T::Encoding, E>;

        { t.begin() } -> Iterator<Rune>;
        { t.end() } -> Same<Empty>;
        { t.Data } -> Same<typename T::Unit const*>;
        { t.Length } -> Same<usize>;
    };
}