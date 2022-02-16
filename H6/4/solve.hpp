#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

// g++ test.cpp -lgtest -lgtest_main -lpthread -O3 && clear && ./a.out

template <typename T>
T calc_length (T num_line) {
    T len = 1;

    if (num_line == 0) {
        throw std::runtime_error ("num_line == 0");
    }

    for (std::size_t i = 1; i < num_line; ++i) {
        len = 2 * len + 1 + (i >= 9);
    }

    return len;
}

template <typename T>
std::size_t solve (T N, T M, T pos) {    
    if (pos == 1) {
        return N < 10 ? N : N / 10;
    }

    const std::size_t len = calc_length (N);

    T cur_len = len;

    // 9 < N < 31
    T len_num = 2;
    while (N >= 10 && pos > 2) {
        pos = std::min (pos - len_num, cur_len - pos + 1);
        cur_len = (cur_len - len_num) / 2;
        --N;
    }

    if (N >= 10) {
        if (pos == 2) {
            return N % 10;
        } else {
            return N / 10;
        }
    }

    // 0 < N < 10
    len_num = 1;
    while (pos != 1) {
        pos = std::min (pos - len_num, cur_len - pos + 1);
        cur_len = (cur_len - len_num) / 2;
        --N;
    }

    return N;
}