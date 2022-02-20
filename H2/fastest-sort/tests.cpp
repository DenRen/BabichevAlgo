#include <gtest/gtest.h>

#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ tests.cpp solve.cpp -O3 -lgtest -lgtest_main -pthread -o test.out

TEST (STATIC_TEST_REF, EXAMPLE_1) {
    ASSERT_EQ (ref_solve (5, 7, 13, 100), 77);
}

TEST (STATIC_TEST, EXAMPLE_1) {
    ASSERT_EQ (solve (5, 7, 13, 100), 77);
}

TEST (RANDOM_GEN_TEST, TEST_SOLVE) {
    seclib::RandomGenerator rand;

    data_t N_min =  5'000'000;
    data_t N_max = 60'000'000;

    std::size_t dim_N = 10;
    std::size_t dim_K = 10;
    std::size_t dim_M = 10;
    std::size_t dim_L = 10;

    for (std::size_t iN = 0; iN < dim_N; ++iN)
    for (std::size_t iK = 0; iK < dim_K; ++iK)
    for (std::size_t iM = 0; iM < dim_M; ++iM)
    for (std::size_t iL = 0; iL < dim_L; ++iL) {
        data_t N = rand.get_rand_val <data_t> (N_min, N_max);
        data_t K = 1 + rand.get_rand_val <data_t> ();
        data_t M = 1 + rand.get_rand_val <data_t> ();
        data_t L = 1 + rand.get_rand_val <data_t> (100'000'000);

        // std::cout << "N: " << N << ", K: " << K << ", M: " << M << ", L: " << L << std::endl;

        data_t ref_res = ref_solve (N, K, M, L);
        data_t res = solve_from_radix (N, K, M, L);

        ASSERT_EQ (ref_res, res)
            << "N: " << N << ", K: " << K << ", M: " << M << ", L: " << L;
    }
}

// TEST (TEST_SORT_COUNT, STATIC) {
//     data_t N = 20000;
//     data_t K = 1000000;
//     data_t M = 23;
//     data_t L = 17;

//     auto seq = calc_seq (N, K, M, L);
//     auto copy_seq = seq;

//     std::sort (seq.begin (), seq.end ());
//     sort_count (copy_seq, L);

//     ASSERT_EQ (seq, copy_seq);
// }

// TEST (CALC_SUM, 1) {
//     data_t N = 9917;
//     data_t K = 913;
//     data_t M = 2003;
//     data_t L = 100001;

//     std::map <data_t, unsigned> map;
//     data_t prev_value = K;
//     ++map[prev_value];
//     for (data_t i = 1; i < N; ++i) {
//         prev_value = next_seq_elem (prev_value, M, L);
//         ++map[prev_value];
//     }
//     data_t sum = sum_seq_even (map, L);

//     std::vector <data_t> vec;
//     vec.reserve (N);
//     for (const auto& [value, freq] : map) {
//         for (unsigned i = 0; i < freq; ++i)
//             vec.push_back (value);
//     }
//     data_t vec_sum = sum_seq_even (vec, L);

//     ASSERT_EQ (sum, vec_sum);
// }