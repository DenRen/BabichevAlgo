#include "solve.hpp"

std::size_t
f (std::size_t x, std::size_t pow) {
    if (pow & 1) {
        return x * f ();
    }
}

std::size_t
fast_pow (std::size_t x, std::size_t pow, std::size_t m) {
    std::size_t res = 1, p = pow;

    while (pow) {
        if (pow & 1) {
            res *= p;
            --pow;
        } else {
            res *= res;
            pow /= 2;
        }
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
    std::cout << fast_pow (X, P, M) << '\n';
}
