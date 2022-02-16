#pragma once

#include <iostream>
#include <cassert>

bool is_prime (int number) {
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}

int solve (int M, int N) {
    const int k_min = 2;
    const int k_max = 20'000'000;

    int l = 2, num_p_l = 1;
    int r = l, num_p_r = 1;

    while (r < k_max) {
        const int dist = r - l + 1;
        if (dist == N) {
            if (num_p_r - num_p_l + 1 == M)
                return l;

            if (is_prime (++l)) {
                ++num_p_l;
            }
        }

        if (dist < N) {

        }
    }

    return -1;
}

int solve_ref (int M, int N) {
    assert (N >  1);
    assert (M >= 0);

    const int k_min = 2;
    const int k_max = 20'000'000;

    for (int k = k_min; k < k_max; ++k) {
        int num_p = 0;
        for (int i = k; i < k + N; ++i) {
            if (is_prime (i)) {
                if (++num_p > M) {
                    break;
                }
            }
        }

        if (num_p == M) {
            return k;
        }
    }

    return -1;
}