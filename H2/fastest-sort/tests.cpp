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
        data_t K = rand.get_rand_val <data_t> ();
        data_t M = rand.get_rand_val <data_t> ();
        data_t L = rand.get_rand_val <data_t> ();

        ASSERT_EQ (ref_solve (N, K, M, L), solve (N, K, L, M));
        std::cout << "1\n";
    }
}