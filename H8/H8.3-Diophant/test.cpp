#include <gtest/gtest.h>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out
/*
TEST (STATIC, SPEC_CASES) {
    bool is_no = false;
    ASSERT_EQ (solve_ax_0_c (2, 4, is_no), 2);
    ASSERT_EQ (solve_ax_0_c (-2, -4, is_no), 2);

    ASSERT_EQ (solve_ax_by_0 (2, 2), std::pair (1l, -1l));
    ASSERT_EQ (solve_ax_by_0 (2, -2), std::pair (1l, 1l));
    ASSERT_EQ (solve_ax_by_0 (10, -3), std::pair (3l, 10l));
    ASSERT_EQ (solve_ax_by_0 (-2, -2), std::pair (1l, -1l));

    ASSERT_EQ (solve_ax_by_c (759, 161, 262913, is_no), std::pair (7l, 1600l));
    ASSERT_EQ (solve_ax_by_c (77, -6, 45486, is_no), std::pair (6l, -7504l));
}

TEST (RANDOM, EXHAUS_SEARCH) {
    seclib::RandomGenerator rand;

    auto vec_a = rand.get_vector <long> (10, 1000);
    auto vec_b = rand.get_vector <long> (10, 1000);
    auto vec_y = rand.get_vector <long> (10, 1000);
    auto vec_x = rand.get_vector <long> (10, 1000);

    for (const auto& a : vec_a)
    for (const auto& b : vec_b)
    for (const auto& y : vec_y)
    for (const auto& x : vec_x) {
        if (x <= 0) {
            continue;
        }

        long c = a * x + b * y;
        auto[is_no, res] = solve_diaf (a, b, c);
        long calc_c = a * res.first + b * res.second;

        ASSERT_EQ (c, calc_c) << "a: " << a << ", b: " << b << ", c: " << c << '\n'
                              << "x: " << x << ", y: " << y << '\n'
                              << "res: " << res.first << ' ' << res.second << '\n';

        ASSERT_TRUE (res.first > 0) << "a: " << a << ", b: " << b << ", c: " << c << '\n'
                                    << "x: " << x << ", y: " << y << '\n'
                                    << "res: " << res.first << ' ' << res.second << '\n';
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
        auto[is_no, res] = solve_diaf (a, b, c);
        ASSERT_FALSE (is_no)
            << "Error is_no == true!\n"
            << a << ' ' << b << ' ' << c << '\n'
            << "y: " << y << '\n';

        ASSERT_FALSE (res.first != x || res.second != y)
            << a << ' ' << b << ' ' << c << '\n'
            << "Error res:" << res.first << ' ' << res.second << '\n'
            << "But ref: " << x << ' ' << y << '\n';
    }
}

TEST (STATIC, AX_BY_EQ_0) {
    long x = 1;

    seclib::RandomGenerator rand;
    auto vec_y = rand.get_vector <int> (10000000);
    auto vec_b = rand.get_vector <int> (10000000);

    std::vector <long> vec_a (vec_y.size ());
    for (std::size_t i = 0; i < vec_y.size (); ++i) {
        vec_a[i] = -1l * vec_y[i] * vec_b[i];
    }

    for (std::size_t i = 0; i < vec_y.size (); ++i) {
        if (vec_a[i] == 0 || vec_b[i] == 0) {
            continue;
        }

        ASSERT_EQ (solve_ax_by_0 (vec_a[i], vec_b[i]), std::pair (x, (long) vec_y[i]));
    }
}*/

long gcd_ (long a, long b, long & x, long & y) {
	if (a == 0) {
		x = 0; y = 1;
		return b;
	}
	long x1, y1;
	long d = gcd_ (b%a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}

bool find_any_solution (long a, long b, long c, long & x0, long & y0) {
	long g = gcd_ (std::abs(a), std::abs(b), x0, y0);
	if (c % g != 0)
		return false;
	x0 *= c / g;
	y0 *= c / g;
	if (a < 0)   x0 *= -1;
	if (b < 0)   y0 *= -1;
	return true;
}

TEST (_, _) {
    seclib::RandomGenerator rand;

    auto vec_a = rand.get_vector <long> (10000, 1000);
    auto vec_b = rand.get_vector <long> (10000, 1000);
    auto vec_c = rand.get_vector <long> (10000, 1000);

    for (const auto& a : vec_a)
    for (const auto& b : vec_b)
    for (const auto& c : vec_c) {
        if ((a == 0 && b == 0) || (a < 0 || b < 0 || c < 0)) {
            continue;
        }

        auto[is_no, res] = solve_diaf (a, b, c);
        long x = res.first, y = res.second;

        long x_ = 0, y_ = 0;
        bool is_no_ = !find_any_solution (a, b, c, x_, y_);

        ASSERT_EQ (is_no, is_no_)
            << a << ' ' << b << ' ' << c << '\n';
        // if (!is_no) {
        //     ASSERT_EQ (x, x_);
        //     ASSERT_EQ (y, y_);
        // }
    }
}
