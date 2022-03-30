#include <gtest/gtest.h>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

TEST (KARP_RABIN, STATIC) {
    auto ptab = calc_ptab (128, 27);

    ASSERT_EQ (ptab[0], 1);
    ASSERT_EQ (ptab[1], 27);
    ASSERT_EQ (ptab[2], 27*27);

    ASSERT_EQ (find_substr_kr ("abaca", "aba", ptab), 0);
    ASSERT_EQ (find_substr_kr ("abaca", "aca", ptab), 2);
    ASSERT_EQ (find_substr_kr ("abacq", "aca", ptab), 5);
}