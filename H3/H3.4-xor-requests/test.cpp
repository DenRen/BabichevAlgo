#include "solve.hpp"

#include <gtest/gtest.h>

TEST (XOR_RANGE, STATIC) {
    using arr_t = xor_array <unsigned>;

    {
        arr_t arr {{10, 0}};
        ASSERT_EQ (arr.xor_range (0, 0), 10);
        ASSERT_EQ (arr.xor_range (1, 1), 0);
        ASSERT_EQ (arr.xor_range (0, 1), 10);
    }
    {
        arr_t arr {{0b0101,
                    0b0100,
                    0b0001}};
        ASSERT_EQ (arr.xor_range (0, 0), 0b0101);
        ASSERT_EQ (arr.xor_range (1, 1), 0b0100);
        ASSERT_EQ (arr.xor_range (2, 2), 0b0001);

        ASSERT_EQ (arr.xor_range (0, 1), 0b0001);
        ASSERT_EQ (arr.xor_range (0, 2), 0b0000);
    }
}

TEST (XOR_RANGE_NATIVE, STATIC) {
    using arr_t = xor_array_native <unsigned>;

    {
        arr_t arr {{10, 0}};
        ASSERT_EQ (arr.xor_range (0, 0), 10);
        ASSERT_EQ (arr.xor_range (1, 1), 0);
        ASSERT_EQ (arr.xor_range (0, 1), 10);
    }
    {
        arr_t arr {{0b0101,
                    0b0100,
                    0b0001}};
        ASSERT_EQ (arr.xor_range (0, 0), 0b0101);
        ASSERT_EQ (arr.xor_range (1, 1), 0b0100);
        ASSERT_EQ (arr.xor_range (2, 2), 0b0001);

        ASSERT_EQ (arr.xor_range (0, 1), 0b0001);
        ASSERT_EQ (arr.xor_range (0, 2), 0b0000);
    }
}

TEST (XOR_RANGE, RANDOM) {
    const auto repeat = 10;

    const auto N_min = 2;
    const auto N_max = 10'000;

    const auto req_xr_min = 2;
    const auto req_xr_max = 10'000;

    seclib::RandomGenerator rand;
    for (std::size_t i_repeat  = 0; i_repeat < repeat; ++i_repeat) {
        const auto N = rand.get_rand_val <unsigned> (N_min, N_max);
        const auto vec = rand.get_vector <unsigned> (N);

        xor_array arr {vec};
        xor_array_native arr_ref {vec};

        std::cout << i_repeat << ": " << N << "\n";
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = i; j < N; ++j) {
                ASSERT_EQ (arr.xor_range (i, j), arr_ref.xor_range (i, j));
            }
        }

    }

}