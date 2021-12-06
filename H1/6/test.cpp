#include <gtest/gtest.h>
#include <random>

#include "other_func.hpp"
#include "solve.hpp"

// g++ test.cpp -lgtest -lgtest_main -lpthread -std=c++17 -O3 && clear && ./a.out

TEST (H1_6, 1) {
    solve ("aab");
}