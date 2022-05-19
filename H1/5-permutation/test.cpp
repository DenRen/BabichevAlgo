#include <gtest/gtest.h>
#include <random>

#include "other_func.hpp"
#include "solve.hpp"

// g++ test.cpp -lgtest -lgtest_main -lpthread -std=c++17 -O3 && clear && ./a.out

std::size_t fact (std::size_t n) {
    std::size_t res = 1;
    while (n > 1) {
        res *= n--;
    }

    return res;
}

TEST (H1_5, up_fact) {
    // for (std::size_t i = 0; i <= 20; ++i) {
    //     std::cout << std::setw (2) << i << ": " << fact (i) << std::endl;
    // }

    ASSERT_EQ (up_fact (1), std::make_pair (1, 1));
    ASSERT_EQ (up_fact (3), std::make_pair (6, 3));
    ASSERT_EQ (up_fact (2), std::make_pair (2, 2));
    ASSERT_EQ (up_fact (720), std::make_pair (720, 6));
    ASSERT_EQ (up_fact (719), std::make_pair (720, 6));
}

TEST (H1_5, explicit) {
    ASSERT_EQ (solve (5, 120), std::vector ({5, 4, 3, 2, 1}));
    ASSERT_EQ (solve (10, 73238), std::vector ({1, 3, 9, 6, 8, 4, 7, 2, 10, 5}));
}

TEST (H1_5, auto_gen) {
    for (std::size_t size = 1; size <= 20; ++size) {
        std::vector <std::size_t> res_ref;
        res_ref.reserve (size);
        for (std::size_t i = size; i > 0; --i) {
            res_ref.push_back (i);
        }

        auto res = solve (size, fact (size));
        EXPECT_EQ (res, res_ref)
            << "size: " << size    << std::endl
            << "res: "  << res     << std::endl
            << "ref: "  << res_ref << std::endl;
    }
}