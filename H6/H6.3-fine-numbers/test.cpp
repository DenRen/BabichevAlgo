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
    ASSERT_EQ (wrapper ({   3}), 1);
    ASSERT_EQ (wrapper ({   7}), 2);

    ASSERT_EQ (wrapper ({3, 3}), 3);
    ASSERT_EQ (wrapper ({3, 7}), 4);
    ASSERT_EQ (wrapper ({7, 3}), 5);
    ASSERT_EQ (wrapper ({7, 7}), 6);

    ASSERT_EQ (wrapper ({3, 3, 3}), 7);
    ASSERT_EQ (wrapper ({3, 3, 7}), 8);
    ASSERT_EQ (wrapper ({3, 7, 3}), 9);
    ASSERT_EQ (wrapper ({3, 7, 7}), 10);

    ASSERT_EQ (wrapper ({7, 3, 3}), 11);
    ASSERT_EQ (wrapper ({7, 3, 7}), 12);
    ASSERT_EQ (wrapper ({7, 7, 3}), 13);
    ASSERT_EQ (wrapper ({7, 7, 7}), 14);
}

class BitEnumerator {
    uint64_t mask = 0;

public:
    bool
    get (int n) const {
        if (n >= sizeof (mask) * 8) {
            throw std::invalid_argument ("overbound request");
        }
        return (mask >> n) & 1;
    }

    void
    operator ++ () {
        ++mask;
    }

    void
    operator ++ (int) {
        ++(*this);
    }
};

TEST (BIT_ENUM, STATIC) {
    BitEnumerator bit_enum;

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