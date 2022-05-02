#include <gtest/gtest.h>
#include <chrono>
#include "geoma.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

TEST (VECTOR_IN_TRIANGLE, STATIC) {
    gtr::Triangle <int> tr {{0, 0}, {0, 10}, {10, 10}};
    std::vector <std::pair <gtr::Vector <int>, bool>> points = {
        {{0, 0}, true},
        {{0, 1}, true},
        {{1, 1}, true},
        {{1, 2}, true},
        {{1, 3}, true},
        {{5, 6}, true},
        {{6, 5}, false},
        {{10, 10}, true},
        {{11, 9}, false}
    };

    for (const auto&[point, state] : points) {
        ASSERT_EQ (gtr::vector_in_triangle (tr, point), state)
            << point;
    }
}