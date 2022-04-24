#pragma once

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <map>
#include <queue>
#include <set>
#include <iomanip>
#include <stack>
#include <fstream>
#include <cmath>
#include <random>

// g++ -DHOST -std=c++17 main.cpp

// #define NDEBUG
#define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cerr << #obj ": " << obj << '\n'
#else
    #define DUMP(obj)
#endif

namespace nrs {

std::vector <bool>
calc_sieve (std::size_t n) {
    std::vector <bool> sieve (n + 1, true);

    for (std::size_t p = 2; p <= n; ++p) {
        if (sieve[p]) {
            for (std::size_t i = p * p; i <= n; i += p) {
                sieve[i] = false;
            }
        }
    }

    return sieve;
}

template <typename T>
std::vector <T>
sieve2vec (const std::vector <bool>& sieve) {
    std::vector <T> vec;
    vec.reserve (1 + std::sqrt (sieve.size ()));

    for (std::size_t i = 0; i < sieve.size (); ++i) {
        if (sieve[i]) {
            vec.push_back (i);
        }
    }

    return vec;
}

template <typename T>
std::vector <T>
calc_primes (std::size_t N) {
    auto sieve = calc_sieve (N);
    auto primes = sieve2vec <T> (sieve);
    return primes;
}

template <typename T>
unsigned
msb (T n) {
    unsigned res = 0;
    while (n >>= 1ull) {
        ++res;
    }

    return res;
}

template <typename T>
T
fast_pow (T x, T pow, T m) {
    T res = 1;

    auto num_bits = msb (pow) + 1;
    T bit_mask = static_cast <T> (pow) << (8 * sizeof (T) - num_bits);

    while (num_bits--) {
        res *= res;
        res %= m;
        T msb_pos = static_cast <T> (1) << (8 * sizeof (T) - 1);
        if (bit_mask & msb_pos) {
            res *= x;
            res %= m;
        }

        bit_mask <<= 1;
    }

    return res;
}

template <typename T>
bool
is_prime_native (T n) { // n >= 0
    if (n <= 3) {
        return true;
    }

    if (n % 2 == 0) {
        return false;
    }

    auto root_n = std::sqrt (n);
    for (T i = 3; i <= root_n; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

// n is odd => n > 0
template <typename T>
bool
is_strong_pseudoprime (T n, unsigned base) {
    // n = d * 2 ^ s + 1
    assert (n > 0);

    if (n == 1) {
        return true;
    }

    --n;

    // calc s
    unsigned s = 1;
    auto d = n / 2;
    while (d % 2 == 0) {
        d /= 2;
        ++s;
    }
    DUMP (s);
    ++n;

    // First check a^d = 1 mod n
    auto a_pow_d = fast_pow <T> (base, d, n);
    if (a_pow_d == 1 || a_pow_d == n - 1) {
        return true;
    }

    // Second check a ^ (a * 2 ^ r) = -1 mod n, 0 <= r < s
    for (unsigned r = 1; r < s; ++r) {
        a_pow_d *= a_pow_d;
        a_pow_d %= n;
        if (a_pow_d == n - 1) {
            return true;
        }
    }

    return false;
}

class is_prime_tester {
    const std::size_t num_primes = 1 << 16;
    std::vector <bool> sieve;

public:
    is_prime_tester () :
        sieve (calc_sieve (num_primes))
    {}

    bool
    operator () (std::size_t N) const {
        return is_prime (N);
    }

    template <typename T>
    bool
    is_prime (T n) const {
        if (n <= num_primes && sieve[n]) {
            return true;
        }

        if (n % 2 == 0) {
            return false;
        }

        /*
        0) n <                             2'047 простое, если оно сильно псевдопростое по основаниям a = 2;
        1) n <                         1'373'653 a = 2, 3;
        2) n <                        25'326'001 a = 2, 3, 5;
        3) n <                     3'215'031'751 a = 2, 3, 5, 7;
        4) n <                 2'152'302'898'747 a = 2, 3, 5, 7, 11;
        5) n <                 3'474'749'660'383 a = 2, 3, 5, 7, 11, 13;
        6) n <               341'550'071'728'321 a = 2, 3, 5, 7, 11, 13, 17;
        7) n <         3'825'123'056'546'413'051 a = 2, 3, 5, 7, 11, 13, 17, 19, 23;
        8) n <   318'665'857'834'031'151'167'461 a = 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37;
        9) n < 3'317'044'064'679'887'385'961'981 a = 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41;

        2 ^ 64 =      18'446'744'073'709'551'615 => max is 8
        */

        unsigned type = 0;
        type += n >=          2047;
        type += n >=     1'373'653;
        type += n >=    25'326'001;
        type += n >= 3'215'031'751;

        if constexpr (sizeof (T) > 4) {
            type += n >=         2'152'302'898'747;
            type += n >=         3'474'749'660'383;
            type += n >=       341'550'071'728'321;
            type += 2 * (n >= 3'825'123'056'546'413'051);
        }

        const unsigned bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
        for (unsigned i = 0; i <= type; ++i) {
            if (is_strong_pseudoprime <T> (n, bases[i]) == false) {
                return false;
            }
        }

        return true;
    }
};

template <typename T, typename Rand>
T
ro_pollard (T n,
            Rand& rand) {
    T x = static_cast <T> (1) + rand () % (n - 2);
    T y = 1, i = 0, stage = 2;

    while (std::gcd (n, x - y) == 1) {
        if (i == stage) {
            y = x;
            stage *= 2;
        }

        x = (x * x + 1) % n;
        ++i;
    }

    return std::gcd (n, x - y);
}

class factorizer {
    is_prime_tester is_prime;
    mutable std::mt19937_64 rand;

    template <typename T>
    void
    put_mults (T n,
               std::vector <T>& mults) const {
        while (n != 1) {
            if (is_prime (n)) {
                mults.push_back (n);
                return;
            }

            T mult = ro_pollard (n, rand);
            if (is_prime (mult)) {
                mults.push_back (mult);
            } else {
                put_mults (mult, mults);
            }

            n /= mult;
        };
    }

public:
    factorizer () :
        rand (std::random_device {} ())
    {}

    template <typename T>
    auto
    operator () (T n) const {
        return to_factorize (n);
    }

    template <typename T>
    std::vector <T>
    to_factorize (T n) const {
        if (n <= 3) {
            return {n};
        }

        std::vector <T> mults;

        // Remove 2^q
        while (n % 2 == 0) {
            mults.push_back (2);
            n /= 2;
        }

        put_mults (n, mults);
        std::sort (mults.begin (), mults.end ());

        return mults;
    }
};

} // namespace nrs

