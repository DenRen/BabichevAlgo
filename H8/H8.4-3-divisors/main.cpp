#include <iostream>
#include <vector>
#include <cmath>

std::vector <bool>
calc_sieve (std::size_t N) {
    std::vector <bool> sieve (N, true);
    sieve[0] = sieve[1] = true;

    std::size_t p = 2;
    while (p * p < N) {
        for (std::size_t k = p * p; k < N; k += p) {
            sieve[k] = false;
        }

        while (++p < N && sieve[p] == 0)
            ;
    }

    return sieve;
}

template <typename T>
std::vector <T>
sieve2vec (const std::vector <bool>& sieve) {
    std::vector <T> vec;
    vec.reserve (sqrt (sieve.size ()));

    for (std::size_t i = 0; i < sieve.size (); ++i) {
        if (sieve[i]) {
            vec.push_back (i);
        }
    }

    return vec;
}

std::ostream&
solve (std::ostream& os,
       std::size_t n) {

    auto sieve = calc_sieve (n++);
    auto vec = sieve2vec <std::size_t> (sieve);
    std::vector <bool> res (n, false);

    for (std::size_t i = 2; i < n; ++i) {
        for (std::size_t j = i + 1; j < n; ++j) {
            for (std::size_t k = j + 1; k < n; ++k) {
                for (std::size_t w = 1; w < n; ++w) {
                    auto p = w * vec[i] * vec[j] * vec[k];
                    if (p < n) {
                        res[p] = true;
                    } else {
                        break;
                    }
                }
                if (vec[i] * vec[j] * vec[k] > n) {
                    break;
                }
            }
            if (vec[i] * vec[j] * vec[j + 1] > n) {
                break;
            }
        }
        if (vec[i] * vec[i + 1] * vec[i + 2] > n) {
            break;
        }
    }

    for (std::size_t i = 2; i < n; ++i) {
        if (res[i]) {
            os << i << ' ';
        }
    }

    return os;
}

int main () {
    std::ios_base::sync_with_stdio (false);

    std::size_t n = 0;
    std::cin >> n;

    solve (std::cout, n) << '\n';
}