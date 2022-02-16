#include <gtest/gtest.h>
#include <random>

#include "other_func.hpp"
#include "solve.hpp"

// g++ test.cpp -lgtest -lgtest_main -lpthread -O3 && clear && ./a.out

std::string gen_seq (std::size_t N) {
    const std::size_t len = calc_length (N);

    std::string res;
    res.reserve (len);
    for (std::size_t i = 1; i <= N; ++i) {
        res += res;
        std::reverse (res.begin () + res.size () / 2, res.end ());
        res = std::to_string (i) + res;
    }

    return res;
}

TEST (H1_3, 1) {
    ASSERT_EQ (solve (1, 1, 1), 1);

    ASSERT_EQ (solve (2, 1, 1), 2);
    ASSERT_EQ (solve (2, 1, 2), 1);
    ASSERT_EQ (solve (2, 1, 3), 1);

    ASSERT_EQ (solve (3, 1, 1), 3);
    ASSERT_EQ (solve (3, 1, 2), 2);
    ASSERT_EQ (solve (3, 1, 3), 1);
    ASSERT_EQ (solve (3, 1, 4), 1);
    ASSERT_EQ (solve (3, 1, 5), 1);
    ASSERT_EQ (solve (3, 1, 6), 1);
    ASSERT_EQ (solve (3, 1, 7), 2);
}

TEST (H1_3, auto_less_10) {
    for (std::size_t N = 1; N < 26; ++N) {
        auto ref_str = gen_seq (N);

        const std::size_t len = ref_str.length ();
        for (std::size_t i = 0; i < len; ++i) {
            ASSERT_EQ (static_cast <int> (ref_str[i] - '0'), solve ((int)N, 1, (int)(i + 1)))
                << "N: " << N << std::endl
                << "i: " << i << std::endl;
        }
    }
}