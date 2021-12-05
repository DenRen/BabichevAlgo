#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include "other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -lpthread -O3 && clear && ./a.out

template <typename T>
std::pair <T, T> up_fact (T number) {
    std::size_t res = 1, n = 1;
    while (number > res) {
        res *= ++n;
    }

    return {res, n};
}

template <typename T>
std::vector <T> solve (const T N, T M) {
    std::vector <T> res;
    res.reserve (N);
    for (std::size_t i = 1; i <= N; ++i) {
        res.push_back (i);
    }

    auto [power, n] = up_fact (M--);
    for (auto pos = res.begin () + N - n; pos != res.end (); ++pos) {
        power /= n--;
        T num = M / power;

        std::swap (*pos, *(pos + num));
        std::sort (pos + 1, res.end ());

        M -= num * power;
        if (M == 0) {
            break;
        }
    }

    return res;
}