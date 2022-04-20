#include <gtest/gtest.h>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

unsigned
msd_native (unsigned n) {
    unsigned r = 0;
    while (n >>= 1) {
        ++r;
    }
    return r;
}

unsigned
msd (unsigned n) {
    unsigned r = 0, k = 16;
    while (n > 1) {
        if (n >> k) {
            n >>= k;
            r += k;
        }
        k /= 2;
    }

    return r;
}

TEST (_, _) {

    for (unsigned i = 0; i < 1 << 22; ++i) {
        ASSERT_EQ (msd_native (i), msd (i)) << i;
    }
}
