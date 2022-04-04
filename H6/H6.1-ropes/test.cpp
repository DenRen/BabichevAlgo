#include <gtest/gtest.h>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

int
calc_len (const std::vector <int>& dists,
          std::vector <bool>& is_rope) {
    int len = 0;
    for (std::size_t i = 0; i < dists.size (); ++i) {
        len += dists[i] * is_rope[i];
    }

    return len;
}

bool
is_correct_bitmap (const std::vector <bool>& bm) {
    if (!bm[0] || !bm[bm.size () - 1]) {
        return false;
    }

    for (int i = 0; i < bm.size () - 1; ++i) {
        if (!bm[i] && !bm[i + 1]) {
            return false;
        }
    }

    return true;
}

int
find_min_len (const std::vector <int>& dists,
              std::vector <bool>& is_rope,
              int pos) {
    if (pos == 0) {
        if (is_correct_bitmap (is_rope)) {
            // std::cout << is_rope << " " << calc_len (dists, is_rope) << '\n';
            return calc_len (dists, is_rope);
        }
        return INT32_MAX;
    }

    int len_1 = find_min_len (dists, is_rope, pos - 1);
    is_rope[pos] = true;
    int len_2 = find_min_len (dists, is_rope, pos - 1);
    is_rope[pos] = false;

    return std::min (len_1, len_2);
}

int
native_solve (const std::vector <int>& nails_) {
    auto nails = nails_;
    auto N = nails.size ();
    std::sort (nails.begin (), nails.end ());

    std::vector <bool> is_rope (N - 1);
    is_rope[N - 2] = true;
    is_rope[0] = true;

    auto& dists = nums2dists (nails);
    return find_min_len (dists, is_rope, N - 2);
}

TEST (NATIVE, STATIC) {
    std::vector <int> v = {{-2, 3, -3, -4, 4, -1}};
    ASSERT_EQ (solve (v), 3);
    v = {-5, -7, 8, 1, -3, -1, 2, -8, 3, -9, -6, 9, 6, 4, -10};
    ASSERT_EQ (solve (v), 10);
}

TEST (RELEASE, STATIC) {
    ASSERT_EQ (native_solve ({-2, 3, -3, -4, 4, -1}), 3);
    ASSERT_EQ (native_solve ({-5, -7, 8, 1, -3, -1, 2, -8, 3, -9, -6, 9, 6, 4, -10}), 10);
}

TEST (RELEASE, RANDOM) {
    int min_size = 4, max_size = 10;
    int num_repeates = 10;

    seclib::RandomGenerator rand;
    for (int i = 0; i < num_repeates; ++i) {
        auto size = rand.get_rand_val <unsigned> (min_size, max_size);
        auto vec = rand.get_vector_uniq <int> (size, 100);

        DUMP (vec);
        EXPECT_EQ (solve (vec), native_solve (vec));
    }
}