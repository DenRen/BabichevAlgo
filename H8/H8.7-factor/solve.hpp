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
#include <chrono>

// g++ -DHOST -std=c++17 main.cpp

// #define NDEBUG
#define HOST

#ifdef HOST 
    #include "../../libs/print_lib.hpp"
    #include "../../libs/other_func.hpp"
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

std::vector <std::size_t>
sieve2vec (const std::vector <bool>& sieve) {
    std::vector <std::size_t> vec;
    vec.reserve (sieve.size ());

    for (std::size_t i = 0; i < sieve.size (); ++i) {
        if (sieve[i]) {    
            vec.push_back (i);
        }
    }

    return vec;
}
/*
template <typename T>
std::ostream&
operator << (std::ostream& os,
             const std::vector <T>& vec)
{
    const std::size_t size = vec.size ();
    if (size == 0) {
        return os;
    }

    for (std::size_t i = 0; i + 1 < size; ++i) {
        os << vec[i] << " ";
    }

    return os << vec[size - 1];
}*/

template <typename T>
T
F (T x, std::size_t N) {
    return (x * x - 1) % N;
}

template <typename Rand>
std::size_t
ro_pollard (std::size_t n,
            Rand& rand) {
    std::size_t x = rand () % 10ull;
    std::size_t y = 1, i = 0, stage = 2;

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

template <typename Rand>
void
put_mults (std::size_t n,
           std::vector <std::size_t>& mults,
           Rand& rand,
           const std::vector <bool>& is_prime) {
    while (n != 1) {
        if ((n & 1) == 0) {
            mults.push_back (2);
            n /= 2;
            continue;
        }

        std::size_t mult = ro_pollard (n, rand); 
        if (is_prime [mult]) {    // n - prime
            mults.push_back (mult);
            n /= mult;
            continue;
        }
        
        put_mults (mult, mults, rand, is_prime);
        n /= mult;
    };
}

std::vector <std::size_t>
solve (std::size_t n) {
    std::vector <std::size_t> mults;
    
    std::random_device rd {};
    std::mt19937 rand (rd ());

    auto sieve = calc_sieve (n);
    put_mults (n, mults, rand, sieve);
    std::sort (mults.begin (), mults.end ());

    return mults;
}

std::vector <std::size_t>
solve2 (std::size_t n) {
    std::vector <std::size_t> mults;
    
    while ((n & 1) == 0) {
        mults.push_back (2);
        n /= 2;
    }

    std::size_t root_n = std::sqrt (n);
    for (std::size_t i = 3; i <= root_n; i += 2) {
        while (n % i == 0) {
            mults.push_back (i);
            n /= i;
            root_n = sqrt (n);
        }
    }

    if (n != 1) {
        mults.push_back (n);
    }

    std::sort (mults.begin (), mults.end ());
    return mults;
}

void
test (std::size_t N) {
    auto ps = sieve2vec (calc_sieve (N));
    auto n = ps.size ();

    for (std::size_t x = 2; x < n; ++x) {
    for (std::size_t y = 2; y < n; ++y) {
    for (std::size_t z = 2; z < n; ++z) {
    for (std::size_t w = 2; w < n; ++w) {
        std::size_t num = ps[x] * ps[y] * ps[z] * ps[w];
        std::vector <std::size_t> mults {ps[x], ps[y], ps[z], ps[w]}; 
        std::sort (mults.begin (), mults.end ());

        auto res = solve2 (num);
        if (res.size () != mults.size () ||
            !std::equal (res.cbegin (), res.cend (), mults.cbegin ())) {
            std::cout << "num: " << num << '\n';
            std::cout << "ref: " << mults <<  '\n';
            std::cout << "res: " << res << '\n';
            std::cout << "F\n";
            return;
        }
    }}}}
}

std::size_t
find_slow (std::size_t ms = 2000) {
    using timer = std::chrono::high_resolution_clock;
    
    seclib::RandomGenerator rand;

    std::size_t time = 0;
    while (time < ms) {
        auto val = rand.get_rand_val <std::size_t> ();

        auto begin = timer::now ();
        auto res = solve2 (val);
        auto end = timer::now ();

        if (auto dt = std::chrono::duration_cast <std::chrono::milliseconds> (end - begin).count ();
            dt >= ms) {
            return val;
        }
    }

    return -1;
}

unsigned
msb (std::size_t n) {
    unsigned res = 0;
    while (n >>= 1) {
        ++res;
    }

    return res;
}

std::size_t
fast_pow (std::size_t x, std::size_t pow, std::size_t m) {
    std::size_t res = 1;
    
    int num_bits = msb (pow) + 1;
    std::size_t bit_mask = pow << (64 - num_bits);

    while (num_bits--) {
        res *= res;
        res %= m;
        if (bit_mask & (1ull << 63)) {
            res *= x;
            res %= m;
        }

        bit_mask <<= 1;
    }

    return res;
}

// n is odd => n > 0
bool
is_strong_presudoprime (std::size_t n, std::size_t base) {
    // n = d * 2 ^ s + 1
    assert (n > 0);
    assert (n % 2 != 0);

    --n;

    // calc s
    unsigned s = 1;
    n /= 2;
    while (n % 2 == 0) {
        n /= 2;
        ++s;
    }

    std::size_t d = n / (1ull << s);
    ++n;
    
    // First check a^d = 1 mod n
    auto a_pow_d = fast_pow (base, d, n);
    if (a_pow_d == 1 || a_pow_d == n - 1) {
        return true;
    }

    // Second check a ^ (a * 2 ^ r) = -1 mod n, 0 <= r < s
    std::size_t pow_2_r = 1;
    for (unsigned r = 1; r < s; ++r) {
        pow_2_r *= 2;
        
    }
}