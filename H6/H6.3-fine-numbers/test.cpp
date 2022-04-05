#include <gtest/gtest.h>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

std::size_t
wrapper (std::initializer_list <int> list) {
    std::vector <int> v {list};
    return solve (v);
}

TEST (SOLVE, STATIC) {
    ASSERT_EQ (wrapper ({      3}), 1);
    ASSERT_EQ (wrapper ({      7}), 2);

    ASSERT_EQ (wrapper ({   3, 3}), 3);
    ASSERT_EQ (wrapper ({   3, 7}), 4);
    ASSERT_EQ (wrapper ({   7, 3}), 5);
    ASSERT_EQ (wrapper ({   7, 7}), 6);

    ASSERT_EQ (wrapper ({3, 3, 3}), 7);
    ASSERT_EQ (wrapper ({3, 3, 7}), 8);
    ASSERT_EQ (wrapper ({3, 7, 3}), 9);
    ASSERT_EQ (wrapper ({3, 7, 7}), 10);

    ASSERT_EQ (wrapper ({7, 3, 3}), 11);
    ASSERT_EQ (wrapper ({7, 3, 7}), 12);
    ASSERT_EQ (wrapper ({7, 7, 3}), 13);
    ASSERT_EQ (wrapper ({7, 7, 7}), 14);
}

TEST (BIT_ENUM, STATIC) {
    seclib::BitEnumerator bit_enum;

    for (int i = 0; i < 64; ++i) {
        ASSERT_FALSE (bit_enum.get (i));
    }

    for (int i = 0; i < 0b11'1111'1111; ++i) {
        bit_enum++;
    }

    for (int i = 0; i < 10; ++i) {
        ASSERT_TRUE (bit_enum.get (i));
    }
}

bool
is_fine_num (std::size_t num) {
    if (num == 0) {
        return false;
    }

    while (num) {
        auto numeric = num % 10;
        if (numeric != 3 && numeric != 7) {
            return false;
        }

        num /= 10;
    }

    return true;
}

bool
is_fine_num (const std::vector <int>& nums) {
    if (nums.size () == 0) {
        return false;
    }

    for (const auto& num : nums) {
        if (num != 3 && num != 7) {
            return false;
        }
    }

    return true;
}

TEST (IS_FINE_NUM, STATIC) {
    ASSERT_TRUE (is_fine_num (3));
    ASSERT_TRUE (is_fine_num (7));
    ASSERT_TRUE (is_fine_num (33));
    ASSERT_TRUE (is_fine_num (37));
    ASSERT_TRUE (is_fine_num (73));
    ASSERT_TRUE (is_fine_num (77));
    ASSERT_TRUE (is_fine_num (33333333));
    ASSERT_TRUE (is_fine_num (3737373777));
    ASSERT_TRUE (is_fine_num (777777777777));

    ASSERT_FALSE (is_fine_num (0));
    ASSERT_FALSE (is_fine_num (1));
    ASSERT_FALSE (is_fine_num (2));
    ASSERT_FALSE (is_fine_num (4));
    ASSERT_FALSE (is_fine_num (10));
    ASSERT_FALSE (is_fine_num (5000000));
    ASSERT_FALSE (is_fine_num (871654641646));
    ASSERT_FALSE (is_fine_num (3333377771));
    ASSERT_FALSE (is_fine_num (17777333777373773));
}

void
write_num_to_vec (std::size_t N, std::vector <int>& buf) {
    assert (N > 0);

    buf.resize (64);
    int i = 0;
    while (N) {
        auto numeric = N % 10;
        buf[i++] = numeric;
        N /= 10;
    }
    buf.resize (i);

    std::reverse (buf.begin (), buf.end ());
}

TEST (RELEASE, AUTO_LITTLE_NUMS) {
    std::size_t N_max = 10'000'000'000 / 1'000'000;

    std::vector <int> buf (64);
    std::size_t num_fine_nums = 0;

    for (std::size_t N = 1; N < N_max; ++N) {
        num_fine_nums += is_fine_num (N);
        write_num_to_vec (N, buf);
        ASSERT_EQ (solve (buf), num_fine_nums) << N;
    }
}

TEST (RELEASE, AUTO_BIG_NUMS) {
    for (int pow_N = 2; pow_N <= 64; ++pow_N) {
        uint64_t num = -1;
        num >>= 64 - pow_N;

        std::size_t num_fine_nums = F (pow_N);
        
        std::vector <int> nums (pow_N, 7);
        assert (is_fine_num (nums));

        ASSERT_EQ (solve (nums), num_fine_nums) << pow_N << ' ' << num;
    }
}