#include <gtest/gtest.h>
#include "numerus.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

TEST (STATIC, SIEVE) {
    const auto N = 1ull << 16;
    std::vector <unsigned> primes_ref {0, 1};
    for (unsigned i = 2; i <= N; ++i) {
        if (is_prime_native (i)) {
            primes_ref.push_back (i);
        }
    }

    auto primes = calc_primes <unsigned> (N);

    ASSERT_EQ (primes.size (), primes_ref.size ());
    ASSERT_TRUE (std::equal (primes.cbegin (), primes.cend (), primes_ref.cbegin ()));
}

TEST (STATIC, IS_STRONG_PSEUDOPRIME) {
    auto check_true = [] (const std::size_t base, const std::vector <std::size_t> nums) {
        std::for_each (nums.cbegin (), nums.cend (), [base] (const auto& n) {
            ASSERT_TRUE (is_strong_pseudoprime (n, base)) << n;
        });
    };

    check_true (2, {2047, 3277, 4033, 4681, 8321, 15841, 29341, 42799,
                    49141, 52633, 65281, 74665, 80581, 85489, 88357, 90751});
    check_true (3, {121, 703, 1891, 3281, 8401, 8911, 10585, 12403, 16531, 18721,
                    19345, 23521, 31621, 44287, 47197, 55969, 63139, 74593, 79003,
                    82513, 87913, 88573, 97567});
    check_true (5, {781, 1541, 5461, 5611, 7813, 13021, 14981, 15751, 24211, 25351,
                    29539, 38081, 40501, 44801, 53971, 79381, 100651, 102311, 104721,
                    112141, 121463, 133141, 141361, 146611, 195313, 211951, 216457,
                    222301, 251521, 289081, 290629, 298271, 315121});
    check_true (97, {49});
    check_true (98, {9});
    check_true (99, {25});
    check_true (128, {49});
}

TEST (FULL_RANGE, IS_PRIME) {
    std::size_t N = 1ull << 18;
    
    is_prime_tester is_prime;

    auto sieve = calc_sieve (N);
    for (std::size_t i = 0; i <= N; ++i) {
        ASSERT_EQ (is_prime (i), sieve[i]) << i;
    }
}