#include "solve.hpp"

#include <gtest/gtest.h>
#include <thread>

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

void
test_xor_range_random (std::size_t repeat) {
    const auto N_min = 2;
    const auto N_max = 500;

    const auto req_xr_min = 2;
    const auto req_xr_max = 100;

    seclib::RandomGenerator rand;
    for (std::size_t i_repeat  = 0; i_repeat < repeat; ++i_repeat) {
        const auto N = rand.get_rand_val <unsigned> (N_min, N_max);
        const auto vec = rand.get_vector <unsigned> (N);

        xor_array arr {vec};
        xor_array_native arr_ref {vec};

        const auto num_req = rand.get_rand_val <unsigned> (req_xr_min, req_xr_max);
        for (unsigned i = 0; i < num_req; ++i) {
            const auto pos = rand.get_rand_val <std::size_t> (N);
            const auto val = rand.get_rand_val <unsigned> ();

            arr.update (pos, val);
            arr_ref.update (pos, val);
        }

        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = i; j < N; ++j) {
                ASSERT_EQ (arr.xor_range (i, j), arr_ref.xor_range (i, j));
            }
        }
    }
}

TEST (XOR_RANGE, RANDOM) {
    auto num_threads = std::thread::hardware_concurrency ();
    if (num_threads == 0) {
        num_threads = 2;
    }

    std::size_t repeat_per_thread = 20000;

    std::vector <std::thread> pool (num_threads);
    for (auto& thread : pool) {
        thread = std::thread (test_xor_range_random, repeat_per_thread);
    }

    for (auto& thread : pool) {
        thread.join ();
    }
}