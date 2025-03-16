#include <Meta/Array.h>
#include <System/Security/Cryptography/Hash.h>
#include <Terminate.h>

namespace System::Crypto {
    static const Array<int, 72> sc_PrimesArray = {
        3,       7,       11,      17,      23,      29,      37,      47,
        59,      71,      89,      107,     131,     163,     197,     239,
        293,     353,     431,     521,     631,     761,     919,     1103,
        1327,    1597,    1931,    2333,    2801,    3371,    4049,    4861,
        5839,    7013,    8419,    10103,   12143,   14591,   17519,   21023,
        25229,   30293,   36353,   43627,   52361,   62851,   75431,   90523,
        108631,  130363,  156437,  187751,  225307,  270371,  324449,  389357,
        467237,  560689,  672827,  807403,  968897,  1162687, 1395263, 1674319,
        2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369
    };
    static const i32 sc_HashPrime = 101;

    always_inline u32 __fmix32(u32 h)
    {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;

        return h;
    }

    always_inline u64 __fmix64(u64 k)
    {
        k ^= k >> 33;
        k *= 0xff51afd7ed558ccdULL;
        k ^= k >> 33;
        k *= 0xc4ceb9fe1a85ec53ULL;
        k ^= k >> 33;

        return k;
    }

    always_inline bool __isprime(i32 candidate)
    {
        if ((candidate & 1) != 0) {
            i32 limit = 0; // TODO: Sqrt(candidate)
            for (int divisor = 3; divisor <= limit; divisor += 2) {
                if ((candidate % divisor) == 0) {
                    return false;
                }
            }
            return true;
        }
        return candidate == 2;
    }

    i32 GetPrime(i32 value)
    {
        if (value < 0) {
            Std::terminate("GetPrime: value must be greater than 0");
        }

        for (i32 i = 0; i < sc_PrimesArray.Length(); i++) {
            int prime = sc_PrimesArray[i];
            if (prime >= value)
                return prime;
        }

        // Outside of our predefined table. Compute the hard way.
        for (i32 i = (value | 1); i < 0x7FFFFFFF; i += 2) {
            if (__isprime(i) and (i - 1) % sc_HashPrime != 0) {
                return i;
            }
        }

        return value;
    }

    u32 MurmurHash3x86(const void* key, usize len, u32 seed)
    {
        u8 const* data    = static_cast<u8 const*>(key);
        int const nblocks = len / 4;

        u32       h1 = seed;
        u32 const c1 = 0xcc9e2d51;
        u32 const c2 = 0x1b873593;

        u32 const* blocks = reinterpret_cast<u32 const*>(data + nblocks * 4);
        for (int i = -nblocks; i; i++) {
            u32 k1 = blocks[i];

            k1 *= c1;
            k1 = (k1 << 15) | (k1 >> 17);
            k1 *= c2;

            h1 ^= k1;
            h1 = (h1 << 13) | (h1 >> 19);
            h1 = h1 * 5 + 0xe6546b64;
        }

        u8 const* tail = data + nblocks * 4;

        u32 k1 = 0;
        switch (len & 3) {
            case 3: {
                k1 ^= tail[2] << 16;
                [[fallthrough]];
            }
            case 2: {
                k1 ^= tail[1] << 8;
                [[fallthrough]];
            }
            case 1: {
                k1 ^= tail[0];
                k1 *= c1;
                k1 = (k1 << 15) | (k1 >> 17);
                k1 *= c2;
                h1 ^= k1;
            }
        }

        h1 ^= len;
        h1 = __fmix32(h1);

        return h1;
    }

    u32 FastHash(i8 const* data, usize len)
    {
        u32 hash = 0, tmp;
        i32 rem  = 0;

        if (len <= 0 or not data) {
            return 0;
        }

        rem = len & 3;
        len >>= 2;

        for (; len > 0; len--) {
            hash += *reinterpret_cast<const u16*>(data);
            tmp  = (*reinterpret_cast<const u16*>(data + 2) << 11) ^ hash;
            hash = (hash << 16) ^ tmp;
            data += 2 * sizeof(u16);
            hash += hash >> 11;
        }

        switch (rem) {
            case 3: {
                hash += *reinterpret_cast<const u16*>(data);
                hash ^= hash << 16;
                hash ^= data[sizeof(u16)] << 18;
                hash += hash >> 11;
                break;
            }
            case 2: {
                hash += *reinterpret_cast<const u16*>(data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
            }
            case 1: {
                hash += *data;
                hash ^= hash << 10;
                hash += hash >> 1;
                break;
            }
            default: {
                break;
            }
        }

        hash ^= hash << 3;
        hash += hash >> 5;
        hash ^= hash << 4;
        hash += hash >> 17;
        hash ^= hash << 25;
        hash += hash >> 6;

        return hash;
    }
}