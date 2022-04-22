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
solve (std::size_t N, std::size_t K, std::size_t P) {
    if (P <= N) {
        return 0;
    }
    
    bool strategy = N - K < K;
    std::size_t L = strategy ? K + 1 : N - K + 1;
    std::size_t D = strategy ? N - K : K;

    std::size_t C = 1;
    for (std::size_t k = L; k <= N; ++k) {
        C *= k;
        C %= P;

        if (C == 0) {
            return 0;
        }
    }
    C %= P;
    
    std::size_t denom = 1;
    for (std::size_t l = 2; l <= D; ++l) {
        denom *= l;
        denom %= P;

        if (denom == 0) {
            return 0;
        }
    }
    denom %= P;

    std::size_t rec = fast_pow (denom, P - 2, P);

    return C * rec % P;
}

int main () {
    std::ios_base::sync_with_stdio (false);

    std::size_t N = 0, K = 0, P = 0;
    std::cin >> N >> K >> P;
    std::cout << solve (N, K, P) << '\n';
}
