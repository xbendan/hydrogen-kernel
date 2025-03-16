#pragma once

#define always_inline __attribute__((always_inline))

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using i8  = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

using uptr = unsigned long long;

using byte  = u8;
using word  = u16;
using dword = u32;
using qword = u64;

using usize = long unsigned int;
using isize = long int;

using f32  = float;
using f64  = double;
using f128 = long double;

using nullptr_t = decltype(nullptr);

struct Empty
{
    constexpr Empty() = default;

    explicit constexpr operator bool() const noexcept { return false; }

    constexpr bool operator==(Empty const&) const = default;

    constexpr bool operator!=(Empty const&) const = default;
};

struct ValueType;

struct ReferenceType;

template <typename T>
bool
operator==(Empty, T* ptr)
{
    return ptr == nullptr;
}

struct Copy
{};

struct Move
{};