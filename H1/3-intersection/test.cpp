#include <gtest/gtest.h>
#include <random>

#include "other_func.hpp"
#include "solve.hpp"

// g++ test.cpp -lgtest -lgtest_main -lpthread -O3 && clear && ./a.out

TEST (H1_3, 1) {
    std::vector <int> a = {1, 3, 4, 5, 7};
    std::vector <int> b = {2, 4, 6, 7, 9};

    auto res = solve (a, b);

    ASSERT_TRUE (res == std::vector <int> ({4, 7})) << res;
}

TEST (H1_3, 2) {
    std::vector <int> a = {1, 3, 4, 5, 7};
    std::vector <int> b = {2, 5, 6, 7, 89};

    auto res = solve (a, b);

    ASSERT_TRUE (res == std::vector <int> ({5, 7})) << res;
}

TEST (H1_3, auto_gen) {
    const std::size_t size_a_min = 1, size_a_max = 1000, size_step_a = 5;
    const std::size_t size_b_min = 1, size_b_max = 1000, size_step_b = 5;
    const std::size_t repeat = 3;

    std::random_device rd;
    std::mt19937 mersenne (rd ());

    for (std::size_t irepeat = 0; irepeat < repeat; ++irepeat) {
    for (std::size_t size_a = size_a_min; size_a < size_a_max; size_a += size_step_a) {
    for (std::size_t size_b = size_b_min; size_b < size_b_max; size_b += size_step_b) {
        auto a = getUniqRandFillVector <int> (size_a, mersenne);
        auto b = getUniqRandFillVector <int> (size_b, mersenne);

        std::sort (a.begin (), a.end ());
        std::sort (b.begin (), b.end ());

        auto res = solve (a, b);
        for (const auto& value : res) {
            if (std::find (a.cbegin (), a.cend (), value) == a.cend () ||
                std::find (b.cbegin (), b.cend (), value) == b.cend ()) {
                ASSERT_TRUE (false)
                    << "a.size (): " << a.size () << std::endl
                    << "b.size (): " << b.size () << std::endl
                    << "a: " << a << std::endl
                    << "b: " << b << std::endl
                    << "res: " << res << std::endl;
            }
        }
    }
    }
    }
}