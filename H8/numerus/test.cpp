#include <gtest/gtest.h>
#include <chrono>
#include "numerus.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

TEST (SIEVE, STATIC) {
    const auto N = 1ull << 16;
    std::vector <unsigned> primes_ref {0, 1};
    for (unsigned i = 2; i <= N; ++i) {
        if (nrs::is_prime_native (i)) {
            primes_ref.push_back (i);
        }
    }

    auto primes = nrs::calc_primes <unsigned> (N);

    ASSERT_EQ (primes.size (), primes_ref.size ());
    ASSERT_TRUE (std::equal (primes.cbegin (), primes.cend (), primes_ref.cbegin ()));
}

TEST (IS_STRONG_PSEUDOPRIME, STATIC) {
    auto check_true = [] (const std::size_t base, const std::vector <std::size_t> nums) {
        std::for_each (nums.cbegin (), nums.cend (), [base] (const auto& n) {
            ASSERT_TRUE (nrs::is_strong_pseudoprime (n, base)) << n;
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

TEST (IS_PRIME, FULL_RANGE) {
    std::size_t N = 1ull << 19;
    
    nrs::is_prime_tester is_prime;

    auto sieve = nrs::calc_sieve (N);
    for (unsigned i = 0; i <= N; ++i) {
        auto i32 = static_cast <uint32_t> (i);
        auto i64 = static_cast <uint64_t> (i);

        ASSERT_EQ (is_prime (i32), sieve[i]) << "uint32: " << i;
        ASSERT_EQ (is_prime (i64), sieve[i]) << "uint64: " << i;
    }
}

TEST (IS_PRIME, FULL_RANGE_SPPED_TEST) {
    std::size_t num_repeat = 1 << 4;
    
    std::size_t N = 1ull << 16;
    auto sieve = nrs::calc_sieve (N);
    nrs::is_prime_tester is_prime;

    for (std::size_t i_repeat = 0; i_repeat < num_repeat; ++i_repeat) {
        for (std::size_t i = 0; i <= N; ++i) {
            volatile bool res = is_prime (i);
            if (res != sieve[i]) {
                std::cout << "F\n";
            }
        }
    }
}

TEST (IS_PRIME, STATIC) {
    nrs::is_prime_tester is_prime;

    ASSERT_TRUE (is_prime (2));
    ASSERT_TRUE (is_prime (3));
    ASSERT_TRUE (is_prime (7));
    ASSERT_TRUE (is_prime (317));
    ASSERT_TRUE (is_prime (27234517));
    ASSERT_TRUE (is_prime (1121764560195671081ull));
    ASSERT_TRUE (is_prime (7803224888578523731ull));
}

TEST (FACTORIZER, STATIC) {
    nrs::factorizer fizer;

    ASSERT_EQ (fizer (2), std::vector <int> {2});
    ASSERT_EQ (fizer (3), std::vector <int> {3});
    ASSERT_EQ (fizer (4), (std::vector <int> {2, 2}));
    ASSERT_EQ (fizer (24), (std::vector <int> {2, 2, 2, 3}));
    ASSERT_EQ (fizer (1234567890127ul), (std::vector <std::size_t> {11, 13, 317, 27234517}));
    ASSERT_EQ (fizer (7852351921369697567ul), (std::vector <std::size_t> {7, 1121764560195671081}));
    ASSERT_EQ (fizer (15606449777157047462ul), (std::vector <std::size_t> {2, 7803224888578523731}));
    ASSERT_EQ (fizer (5333648285106940643ul), (std::vector <std::size_t> {13, 37, 28074271, 394976093}));
}

TEST (FACTORIZER, RANDOM_SLOW) {
    std::size_t n = 1ull << 6;
    
    nrs::is_prime_tester is_prime;
    nrs::factorizer fizer;
    seclib::RandomGenerator rand;
    
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t num = rand.get_rand_val <std::size_t> (1000000);
        if (num == 0) {
            --i;
            continue;
        }

        auto mults = fizer (num);
        std::size_t res = 1;
        std::for_each (mults.cbegin (), mults.cend (), [&] (const auto& mult) {
            res *= mult;
            ASSERT_TRUE (is_prime (mult));
        });

        ASSERT_EQ (num, res);
    }
}

TEST (NUM_COMB, STATIC) {
    ASSERT_EQ (nrs::num_comb (10, 2, 10007), 45);
    ASSERT_EQ (nrs::num_comb (10, 4, 10007), 210);
    ASSERT_EQ (nrs::num_comb (789799879ul, 878989ul, 797979879797979ul), 165387439016947);

    std::size_t res = 0;
    for (std::size_t i = 0; i <= 10; ++i) {
        res += nrs::num_comb (10, i, 10007);
    }
    ASSERT_EQ (res, 1024);
}

namespace sum_num_comb {

auto check = [] (std::size_t n, std::size_t l, std::size_t r, std::size_t m) {
    ASSERT_EQ (nrs::sum_num_comb (n, l, r, m),
               nrs::sum_num_comb_native (n, l, r, m))
               << "failed n: " << n << ", l: " << l << ", r: " << r << ", m: " << m;
};

TEST (SUM_NUM_COMB, STATIC) {
    auto check_native = [] (std::size_t n, std::size_t l, std::size_t r, std::size_t m, std::size_t res) {
        ASSERT_EQ (nrs::sum_num_comb_native (n, l, r, m), res);
    };

    check_native (10, 1, 10, 10007, 1024 - 1);
    check_native (12, 1, 12, 10007, 4096 - 1);

    ASSERT_EQ (nrs::detail::sum_num_comb_calc_k0 (10, 1, 10), 5);
    ASSERT_EQ (nrs::detail::sum_num_comb_calc_k0 (24, 2, 3), 4);

    check (10, 1, 10, 10007);
    check (12, 1, 12, 10007);
    check (13, 3, 12, 10007);
    check (20, 2, 3, 10007);
    check (40, 2, 9, 10007);
    check (119, 19, 2, 10007);

    for (std::size_t n = 3; n < 50*4; ++n) {
        for (std::size_t r = 1; r <= 50; ++r) {
            for (std::size_t l = 5; l < 30; ++l) {
                check (n, l, r, 2);
                check (n, l, r, 3);
                check (n, l, r, 5);
                check (n, l, r, 13);
                check (n, l, r, 10007);
                check (n, l, r, 1000000007);
            }
        }
    }
}

TEST (SUM_NUM_COMB, RANDOM) {
    seclib::RandomGenerator rand;

    auto primes = nrs::calc_primes <std::size_t> (1000000);
    std::reverse (primes.begin (), primes.end ());
    primes.resize (sqrt (primes.size ()));

    std::size_t num_repeat = 1'000'000;
    for (std::size_t i_repeat = 0; i_repeat < num_repeat; ++i_repeat) {
        auto n = rand.get_rand_val <std::size_t> ();
        auto l = rand.get_rand_val <std::size_t> ();
        auto r = rand.get_rand_val <std::size_t> ();

        for (const auto& m : primes) {
            check (n, l, r, m);
        }

        check (n, l, r, 10007);
        check (n, l, r, 1000000007);
    }
}

TEST (SUM_NUM_COMB, SPEED_TEST) {
    for (std::size_t n = 1ull << 10; n < 1ull << 24; n *= 2) {
        for (std::size_t r = n / 16; r <= n / 2; r *= 2) {
            for (std::size_t l = 1; l < 1ull << 10; l = 2 * l + 1) {
                volatile std::size_t res = 0;
                res += nrs::sum_num_comb (n, l, r, 2);
                res += nrs::sum_num_comb (n, l, r, 3);
                res += nrs::sum_num_comb (n, l, r, 5);
                res += nrs::sum_num_comb (n, l, r, 13);
                res += nrs::sum_num_comb (n, l, r, 10007);
                res += nrs::sum_num_comb (n, l, r, 1000000007);
                if (res == -1ull) {
                    ASSERT_TRUE (false);
                }
            }
        }
    }
}

TEST (BIT_FUNCS, STATIC) {
    ASSERT_FALSE (nrs::is_pow_2 (0));

    for (std::size_t i = 1; i < 64; ++i) {
        ASSERT_TRUE (nrs::is_pow_2 (1ull << i));
        ASSERT_FALSE (nrs::is_pow_2 (1 + (1ull << i)));
    }

    EXPECT_EQ (nrs::increase_pow_2 (0), 1);
    EXPECT_EQ (nrs::increase_pow_2 (1), 1);
    EXPECT_EQ (nrs::increase_pow_2 (1ull << 4), 1ull << 4);
    EXPECT_EQ (nrs::increase_pow_2 (1ull << 33), 1ull << 33);
    EXPECT_EQ (nrs::increase_pow_2 (1 + (1ull << 4)), 1ull << 5);
    EXPECT_EQ (nrs::increase_pow_2 (1 + (1ull << 33)), 1ull << 34);

    EXPECT_EQ (nrs::increase_pow_2 (5), 8);
    EXPECT_EQ (nrs::increase_pow_2 (7), 8);
    EXPECT_EQ (nrs::increase_pow_2 (8), 8);
}

template <typename T, typename U>
void
is_equal_fp_vec (const std::vector <T>& lhs,
                 const std::vector <U>& rhs) {
    ASSERT_EQ (lhs.size (), rhs.size ()) << "lhs: " << lhs << ", rhs: " << rhs;

    for (std::size_t i = 0; i < lhs.size (); ++i) {
        ASSERT_LE (std::abs (lhs[i] - rhs[i]), 1e-4) << "lhs: " << lhs << ", rhs: " << rhs;
    }
}

TEST (FFT_FFT_NATIVE, STATIC) {
    auto check = [] (const std::vector <double>& P,
                     const std::vector <double>& Q,
                     const std::vector <double>& R) {
        auto PQ_native = nrs::fft::mult_poli_native (P, Q);
        auto P_copy = P, Q_copy = Q;
        auto PQ = nrs::fft::mult_poli (P_copy, Q_copy);
        is_equal_fp_vec (PQ_native, R);
        is_equal_fp_vec (PQ, R);
    };

    check ({1, 0, 2}, {2, 3}, {2, 3, 4, 6});
    check ({1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 2, 3, 4, 5, 4, 3, 2, 1});
}

TEST (FFT, RANDOM) {
    const auto num_repeat = 1000;
    const auto n_min = 1, n_max = 10000;
    const auto m_min = 1, m_max = 10000;
    const int mod = 10;
    
    seclib::RandomGenerator rand;
    auto n_arr = rand.get_vector <unsigned> (num_repeat, n_min, n_max);
    auto m_arr = rand.get_vector <unsigned> (num_repeat, m_min, m_max);

    for (int i_repeat = 0; i_repeat < num_repeat; ++i_repeat) { 
        auto n = n_arr[i_repeat], m = m_arr[i_repeat];

        auto P = rand.get_vector <double> (n, mod);
        auto Q = rand.get_vector <double> (m, mod);
        
        auto R_ref = nrs::fft::mult_poli_native (P, Q);
        auto R = nrs::fft::mult_poli (P, Q);
        is_equal_fp_vec (R_ref, R);
    }
}

TEST (FFT_SPEED_TEST, RANDOM) {
    using timer = std::chrono::high_resolution_clock;
    // using dur = std::chrono::duration_cast;

    seclib::RandomGenerator rand;

    auto P = rand.get_vector <double> (100000, 10);
    auto Q = rand.get_vector <double> (100000, 10);

    const auto num_repeats = 10;
    auto begin = timer::now ();
    for (int i = 0; i < num_repeats; ++i) {
        volatile auto R = nrs::fft::mult_poli (P, Q);
    }
    auto end = timer::now ();

    auto delta = std::chrono::duration_cast <std::chrono::milliseconds> (end - begin).count ();
    delta /= num_repeats;

    ASSERT_TRUE (delta < 50) << delta;
}

TEST (FFT_OTHER_FUNCS, STATIC) {
    {
        std::vector <int> vals {0, 1};
        nrs::fft::conv2first_line (vals, 1);
        ASSERT_EQ (vals, (decltype (vals) {0, 1}));
    }
    {
        std::vector <int> vals {0, 1, 2, 3};
        nrs::fft::conv2first_line (vals, 2);
        ASSERT_EQ (vals, (decltype (vals) {0, 2, 1, 3}));
    }
    {
        std::vector <int> vals {0, 1, 2, 3, 4, 5, 6, 7};
        nrs::fft::conv2first_line (vals, 3);
        ASSERT_EQ (vals, (decltype (vals) {0, 4, 2, 6, 1, 5, 3, 7}));
    }
    {
        std::vector <int> vals {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        nrs::fft::conv2first_line (vals, 4);
        ASSERT_EQ (vals, (decltype (vals) {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15}));
    }
}

} // namespace sum_num_comb