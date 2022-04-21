#define NDEBUG
#include <iostream>
#include <numeric>
#include <tuple>

bool
is_eq_sign (long a, long b) {
    return !((a & (1ull << 63)) ^ (b & (1ull << 63)));
}

std::tuple <long, long, long>
ext_euclid (long a, long b) {
    long u1 = 1, v1 = 0, u2 = 0, v2 = 1;
    while (b != 0) {
        long q = a / b, r = a % b;
        a = b;
        b = r;

        long u1_ = u2, v1_ = v2;
        u2 = u1 - q * u2;
        v2 = v1 - q * v2;

        u1 = u1_;
        v1 = v1_;
    }

    return {a, u1, v1};
}

std::tuple <bool, long, long>
solve_diaf (long a, long b, long c) {
    bool is_no = false;
    long x = 0, y = 0;

    if (a == 0 && b == 0) {
        x = 1;
        is_no = c != 0;
    } else {
        long abs_b = std::abs (b);
        auto[d, u, v] = ext_euclid (a, b);  // 1 -1 1
        if (c % d == 0) {
            if (b != 0) {
                long dx = std::abs (abs_b / d), c_ = c / d;
                long x0 = u * c_;

                x = (dx + x0 % dx) % dx;
                if (x == 0) {
                    x = dx;
                }
                
                y = b == 0 ? 0 : (c - a * x) / b;
            } else {
                if (is_eq_sign (a, c)) {
                    x = c / a;
                } else {
                    is_no = true;
                }
            }
        } else {
            is_no = true;
        }
    }

    return {is_no, x, y};
}