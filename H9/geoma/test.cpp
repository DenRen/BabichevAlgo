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

TEST (IS_EQ_LESS_GREATER, STATIC) {
    auto test = [] (auto a, auto b) {
        ASSERT_TRUE  (gtr::is_equal (a, a));
        ASSERT_FALSE (gtr::is_equal (a, b));

        ASSERT_TRUE  (gtr::is_less (a, b));
        ASSERT_FALSE (gtr::is_less (b, a));
        ASSERT_FALSE (gtr::is_less (a, a));

        ASSERT_TRUE  (gtr::is_less_eq (a, a));
        ASSERT_TRUE  (gtr::is_less_eq (a, b));
        ASSERT_FALSE (gtr::is_less_eq (b, a));

        ASSERT_TRUE  (gtr::is_greater (b, a));
        ASSERT_FALSE (gtr::is_greater (a, b));
        ASSERT_FALSE (gtr::is_greater (a, a));

        ASSERT_TRUE  (gtr::is_greater_eq (a, a));
        ASSERT_TRUE  (gtr::is_greater_eq (b, a));
        ASSERT_FALSE (gtr::is_greater_eq (a, b));
    };

    test (1, 2);
    
    test (1.0, 2);
    test (1, 2.0);
    test (1.0, 2.0);

    test (1.0f, 2);
    test (1, 2.0f);
    test (1.0f, 2.0f);

    test (1.0f, 1.00002);
    test (1.0f, 1.00009);
}