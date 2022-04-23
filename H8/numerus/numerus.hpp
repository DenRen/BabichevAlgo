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
unsigned
msb (T n) {
    unsigned res = 0;
    while (n >>= 1u) {
        ++res;
    }

    return res;
}

template <typename T>
T
fast_pow (T x, unsigned pow, T m) {
    T res = 1;

    auto num_bits = msb (pow) + 1;
    std::size_t bit_mask = static_cast <T> (pow) << (8 * sizeof (T) - num_bits);

    while (num_bits--) {
        res *= res;
        res %= m;
        if (bit_mask & (static_cast <T> (1) << (8 * sizeof (T) - 1))) {
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
is_strong_pseudoprime (T n, T base) {
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

    ++n;

    // First check a^d = 1 mod n
    auto a_pow_d = fast_pow (base, d, n);
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