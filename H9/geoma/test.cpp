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

TEST (IS_INTERSECT_LS_LS, STATIC) {
    {
        gtr::LineSegment <double> ls1 {{1, 1}, {7, 4}},
                                  ls2 {{5, 3}, {3, 2}};
        EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    }
    {
        gtr::LineSegment <double> ls1 {{-1, -1}, {1, 1}},
                                  ls2 {{-2, -2}, {2, 2}};
        EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    }
    {
        gtr::LineSegment <double> ls1 {{-1, -1}, {-2, -2}},
                                  ls2 {{+2, +2}, {-0.999, -0.999}};
        EXPECT_FALSE (gtr::is_intersect (ls1, ls2));
    }
    {
        gtr::LineSegment <double> ls1 {{1, 4}, {3, 5}},
                                  ls2 {{5, 6}, {7, 7}};
        EXPECT_FALSE (gtr::is_intersect (ls1, ls2));
    }
    {
        gtr::LineSegment <double> ls1 {{-1, -1}, {0, 0}},
                                  ls2 {{0.1, 0.1}, {3, 2}};
        EXPECT_FALSE (gtr::is_intersect (ls1, ls2));
    }
    {
        gtr::LineSegment <double> ls1 {{-1, -10}, {5, 11.76}},
                                  ls2 {{5, -10}, {-20, 30}};
        EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    }
    {
        gtr::LineSegment <double> ls1 {{-1, -1}, {-0.1, -0.1}},
                                  ls2 {{-0.05, -0.05}, {20, 30}};
        EXPECT_FALSE (gtr::is_intersect (ls1, ls2));
    }
    {
        gtr::LineSegment <double> ls1 {{-3, 0}, {6, 3}},
                                  ls2 {{3, 2}, {5, 0}};
        EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    }
    {
        gtr::LineSegment <double> ls1 {{-3e5, 0}, {6e5, 3e5}},
                                  ls2 {{3e5, 2e5}, {5e5, 0}};
        EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    }
    {
        gtr::LineSegment <double> ls1 {{-3, 0}, {6, 3}},
                                  ls2 {{4, 2}, {5, 0}};
        EXPECT_FALSE (gtr::is_intersect (ls1, ls2));
    }
    {
        gtr::LineSegment <double> ls1 {{-3, 0}, {3, 2}},
                                  ls2 {{3, 2}, {5, 0}};
        EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    }
}