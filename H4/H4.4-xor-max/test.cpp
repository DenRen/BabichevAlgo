#include <gtest/gtest.h>

#include "../../libs/other_func.hpp"
#include "solve.hpp"

TEST (XOR_MAX, RANDOM) {
    seclib::RandomGenerator rand;

    const auto N_min =      10;
    const auto N_max = 500'000 / 100;

    const unsigned value_min = 0;
    const unsigned value_max = 2 * N_max;

    const std::size_t num_repeats = 10;
    for (std::size_t num_repeat = 0; num_repeat < num_repeats; ++num_repeat) {
        const auto N = rand.get_rand_val <unsigned> (N_min, N_max);

        xor_array_bt arr;
        // xor_array_map arr;
        xor_array_native ref;
        // Insert
        for (unsigned i = 0; i < N; ++i) {
            auto value = rand.get_rand_val <unsigned> (value_min, value_max);
            arr.insert (value);
            ref.insert (value);
        }

        for (unsigned i = 0; i < N; ++i) {
            auto value = rand.get_rand_val <unsigned> (value_min, value_max);
            ASSERT_EQ (arr.max_xor (value), ref.max_xor (value)) << value;
        }

        // Remove
        for (unsigned i = 0; i < N / 10; ++i) {
            auto value = rand.get_rand_val <unsigned> (value_min, value_max);
            arr.remove (value);
            ref.remove (value);
        }

        for (unsigned i = 0; i < N / 10; ++i) {
            auto value = rand.get_rand_val <unsigned> (value_min, value_max);
            ASSERT_EQ (arr.max_xor (value), ref.max_xor (value));
        }
    }
    
}