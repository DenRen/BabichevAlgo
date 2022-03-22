#include <gtest/gtest.h>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

TEST (_, _) {
    seclib::RandomGenerator rand;
    tree_t tree;

    const auto str_len = 1000;
    std::cout << solve (rand.get_string (str_len, 'a', 'b')) << "\n";
}