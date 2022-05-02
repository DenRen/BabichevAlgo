#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>

std::vector <bool>
calc_sieve (std::size_t n) {
    std::vector <bool> sieve (n + 1, true);

    for (std::size_t p = 2; p <= n; ++p) {
        if (sieve[p]) {
            for (std::size_t i = p * p; i <= n; i += p) {
                sieve[i] = false;
            }
        }
    }

    return sieve;
}

template <typename T>
std::vector <T>
sieve2vec (const std::vector <bool>& sieve) {
    std::vector <T> vec;
    vec.reserve (sieve.size ());

    for (std::size_t i = 0; i < sieve.size (); ++i) {
        if (sieve[i]) {
            vec.push_back (i);
        }
    }

    return vec;
}

template <typename T>
T
solve_native (T n) {
    T res = 2 * (n / 2);

    auto sieve = calc_sieve (n);
    auto primes = sieve2vec <T> (sieve);

    for (std::size_t i = 3; i <= n; i += 2) {
        for (std::size_t j = 2; j <= n; ++j) {
            if (i % primes[j] == 0) {
                res += primes[j];
                break;
            }
        }
    }

    return res;
}

template <typename T>
T
solve (T n) {
    T res = 2 * (n / 2);

    auto sieve = calc_sieve (n);
    
    for (T i = 3; i < n; ++i) {
        res += sieve[i] ? i : 0;
    }

    for (T i = 3; i < n; ++i) {
        if (sieve[i]) {
            auto p = i;

            auto accum = p * p;
            while (accum <= n) {
                if (sieve[accum] == false) {
                    sieve[accum] = true;
                    res += p;
                }
                accum += 2 * p;
            }
        }
    }

    return res;
}

void test (std::size_t N) {
    for (std::size_t n = 1; n <= N; n *= 2) {
        auto res = solve <std::size_t> (n);
        auto res_ref = solve_native <std::size_t> (n);

        if (res != res_ref) {
            std::cout << "F: " << n << '\n';
            std::cout << "ref: " << res_ref << '\n';
            std::cout << "res: " << res << '\n';
            return;
        }
    }
}

int main () {
    std::ios_base::sync_with_stdio (false);

    std::size_t n = 0;
    std::cin >> n;

    // test (n);
    // return 0;

    // std::cout << solve_native (n) << std::endl;
    std::cout << solve <std::size_t> (n) << std::endl;
}
