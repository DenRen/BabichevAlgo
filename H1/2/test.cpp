#include <gtest/gtest.h>
#include <random>

#include "other_func.hpp"
#include "solve.hpp"

// g++ test.cpp -lgtest -lgtest_main -lpthread -O3 && clear && ./a.out

template <typename T>
std::vector <T> genResult (const std::vector <T>& lhs, const std::vector <T>& rhs) {
    std::vector <T> concat = lhs;
    concat.reserve (lhs.size () + rhs.size ());

    for (const auto& item : rhs) {
        concat.push_back (item);
    }

    const std::size_t size = concat.size ();
    if (size <= 1) {
        return concat;
    }

    // concat.size >= 2
    std::sort (concat.begin (), concat.end ());

    std::vector <T> res;
    res.reserve (concat.size ());

    std::size_t i = 0;
    for (; i + 1 < size; ++i) {
        if (concat[i] != concat[i + 1]) {
            res.push_back (concat[i]);
        } else {
            ++i;
        }
    }

    if (i == size - 1) {
        res.push_back (concat[i]);
    }

    return res;
}

TEST (H1_2, input_empty) {
    std::vector <int> a;
    std::vector <int> b;
    std::vector <int> res_ref;

    auto res = solve (a, b);
    ASSERT_TRUE (res_ref == res) << res;
}

TEST (H1_2, input_single_empty) {
    std::vector <int> a;
    std::vector <int> b       = {4, 1, 6, 2, 3, 9};
    std::vector <int> res_ref = {1, 2, 3, 4, 6, 9};

    auto res = solve (a, b);
    ASSERT_TRUE (res_ref == res) << res;

    res = solve (b, a);
    ASSERT_TRUE (res_ref == res) << res;
}

TEST (H1_2, 1) {
    std::vector <int> a = {1, 2, 3, 4, 5};
    std::vector <int> b = {1, 7, 5, 8};

    auto res_ref = genResult (a, b);
    auto res = solve (a, b);

    ASSERT_TRUE (res_ref == std::vector <int> ({2, 3, 4, 7, 8})) << res_ref;
    ASSERT_TRUE (res_ref == res) << res;
}

TEST (H1_2, 2) {
    std::vector <int> a = {1, 2, 6, 8, 7, 3};
    std::vector <int> b = {4, 1, 6, 2, 3, 9};

    auto res_ref = genResult (a, b);
    auto res = solve (a, b);

    ASSERT_TRUE (res_ref == std::vector <int> ({4, 7, 8, 9})) << res_ref;
    ASSERT_TRUE (res_ref == res) << res;
}

TEST (H1_2, 3) {
    std::vector <int> a = {1, 2, 3, 4, 7, 57};
    std::vector <int> b = {1, 2, 4, 5, 6, 9};

    auto res_ref = genResult (a, b);
    auto res = solve (a, b);

    ASSERT_TRUE (res_ref == std::vector <int> ({3, 5, 6, 7, 9, 57})) << res_ref;
    ASSERT_TRUE (res_ref == res) << res;
}

TEST (H1_2, auto_gen) {
    const std::size_t size_a_min = 1, size_a_max = 1000, size_step_a = 10;
    const std::size_t size_b_min = 1, size_b_max = 1000, size_step_b = 10;
    const std::size_t repeat = 3;

    std::random_device rd;
    std::mt19937 mersenne (rd ());

    for (std::size_t irepeat = 0; irepeat < repeat; ++irepeat) {
    for (std::size_t size_a = size_a_min; size_a < size_a_max; size_a += size_step_a) {
    for (std::size_t size_b = size_b_min; size_b < size_b_max; size_b += size_step_b) {
        auto a = getUniqRandFillVector <int> (size_a, mersenne, 10 * size_a_max);
        auto b = getUniqRandFillVector <int> (size_b, mersenne, 10 * size_b_max);

        {
            auto res_ref = genResult (a, b);
            auto res = solve (a, b);
            ASSERT_TRUE (res_ref == res)
                << "a.size (): " << a.size () << std::endl
                << "b.size (): " << b.size () << std::endl
                << "a: " << a << std::endl
                << "b: " << b << std::endl
                << "res: " << res << std::endl
                << "ref: " << res_ref << std::endl;
        }

        {
            auto res_ref = genResult (b, a);
            auto res = solve (b, a);
            ASSERT_TRUE (res_ref == res) << res;
        }
    }
    }
    }
}