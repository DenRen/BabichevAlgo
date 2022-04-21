#include <gtest/gtest.h>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

TEST (RANDOM, EXHAUS_SEARCH) {
    seclib::RandomGenerator rand;

    auto vec_a = rand.get_vector <long> (100, 1000);
    auto vec_b = rand.get_vector <long> (100, 1000);
    auto vec_y = rand.get_vector <long> (100, 1000);
    auto vec_x = rand.get_vector <long> (100, 1000);

    for (const auto& a : vec_a)
    for (const auto& b : vec_b)
    for (const auto& y : vec_y)
    for (const auto& x : vec_x) {
        if (x <= 0) {
            continue;
        }
        
        long c = a * x + b * y;
        auto[is_no, x_, y_] = solve_diaf (a, b, c);
        long calc_c = a * x_ + b * y_;

        ASSERT_EQ (c, calc_c) << "a: " << a << ", b: " << b << ", c: " << c << '\n'
                              << "x: " << x << ", y: " << y << '\n'
                              << "res: " << x_ << ' ' << y_ << '\n';

        ASSERT_TRUE (x_ > 0) << "a: " << a << ", b: " << b << ", c: " << c << '\n'
                                    << "x: " << x << ", y: " << y << '\n'
                                    << "res: " << x_ << ' ' << y_ << '\n';
    }
}

TEST (RANDOM, X_EQ_1) {
    seclib::RandomGenerator rand;

    auto vec_a = rand.get_vector <int> (100, 1000);
    auto vec_b = rand.get_vector <int> (100, 1000);
    auto vec_y = rand.get_vector <int> (100, 1000);
    long x = 1;

    for (const auto& a : vec_a)
    for (const auto& b : vec_b)
    for (const auto& y : vec_y) {
        if (a == 0 || b == 0) {
            continue;
        }

        long c = (long) a * x + (long) b * y;
        auto[is_no, x_, y_] = solve_diaf (a, b, c);
        ASSERT_FALSE (is_no)
            << "Error is_no == true!\n"
            << a << ' ' << b << ' ' << c << '\n'
            << "y: " << y << '\n';

        ASSERT_FALSE (x_ != x || y_ != y)
            << a << ' ' << b << ' ' << c << '\n'
            << "Error res:" << x_ << ' ' << y_ << '\n'
            << "But ref: " << x << ' ' << y << '\n';
    }
}
