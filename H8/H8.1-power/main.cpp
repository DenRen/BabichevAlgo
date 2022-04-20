#include <iostream>

int
msb (std::size_t n) {
    int res = 0;
    while (n >>= 1) {
        ++res;
    }

    return res;
}

std::size_t
fast_pow (std::size_t x, std::size_t pow, std::size_t m) {
    std::size_t res = 1;
    
    int num_bits = msb (pow) + 1;
    std::size_t bit_mask = pow << (64 - num_bits);

    while (num_bits--) {
        res *= res;
        res %= m;
        if (bit_mask & (1ull << 63)) {
            res *= x;
            res %= m;
        }

        bit_mask <<= 1;
    }

    return res;
}

std::size_t
solve (std::size_t X, std::size_t P, std::size_t M) {
    return fast_pow (X % M, P, M);
}

int main () {
    std::ios_base::sync_with_stdio (false);

    std::size_t X = 0, P = 0, M = 0;
    std::cin >> X >> P >> M;
    std::cout << solve (X, P, M) << '\n';
}
