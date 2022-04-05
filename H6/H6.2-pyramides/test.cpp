#include <gtest/gtest.h>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

TEST (MIN_BASE, STATIC) {
    min_base_t mb {255};

    EXPECT_EQ (mb.get_base ( 1), 1);
    EXPECT_EQ (mb.get_base ( 2), 2);
    EXPECT_EQ (mb.get_base ( 3), 2);
    EXPECT_EQ (mb.get_base ( 4), 3);
    EXPECT_EQ (mb.get_base ( 5), 3);
    EXPECT_EQ (mb.get_base ( 6), 3);
    EXPECT_EQ (mb.get_base ( 7), 4);
    EXPECT_EQ (mb.get_base (10), 4);
    EXPECT_EQ (mb.get_base (11), 5);
}

TEST (F, STATIC) {
    F_t F;
    EXPECT_EQ (F (1), 1);
    EXPECT_EQ (F (2), 1);
    EXPECT_EQ (F (3), 2); // std::cout << '\n';
    EXPECT_EQ (F (4), 2); // std::cout << '\n';
    EXPECT_EQ (F (5), 3); // std::cout << '\n';
    EXPECT_EQ (F (6), 4); // std::cout << '\n';
    EXPECT_EQ (F (7), 5); // std::cout << '\n';
}