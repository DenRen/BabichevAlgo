#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include "solve.hpp"

std::vector <bool>
calc_sieve (std::size_t N) {
    std::vector <bool> sieve (N + 1, true);

    for (std::size_t p = 2; p <= N; p++) {
        if (sieve[p]) {
            for (std::size_t j = p * p; j <= N; j += p) {
                sieve[j] = false;
            }
        }
    }

    return sieve;
}

std::vector <std::size_t>
sieve2vec (const std::vector <bool>& sieve) {
    std::vector <std::size_t> vec;
    vec.reserve (sqrt (sieve.size ()));

    for (std::size_t i = 0; i < sieve.size (); ++i) {
        if (sieve[i]) {
            vec.push_back (i);
        }
    }

    return vec;
}

bool
is_3_prim (std::size_t num, const std::vector <std::size_t>& primes) {
    std::set <int> mul;

    while (num > 1) {
        for (std::size_t i = 2; i < primes.size (); ++i) {
            auto p = primes[i];
            if (num % p == 0) {
                mul.insert (p);
                num /= p;
            }

            if (num == 1) {
                break;
            }
        }
    }

    return mul.size () >= 3;
}

std::vector <bool>
solve (std::size_t n, const std::vector <std::size_t>& vec) {
    std::vector <bool> res (n + 1, false);
    
    auto num_primes = vec.size ();
    for (std::size_t i = 2; i < num_primes; ++i) {
        if (vec[i] * vec[i] * vec[i] > n) {
            break;
        }

        for (std::size_t j = i + 1; j < num_primes; ++j) {
            if (vec[i] * vec[j] * vec[j] > n) {
                break;
            }

            for (std::size_t k = j + 1; k < num_primes; ++k) {
                if (vec[i] * vec[j] * vec[k] > n) {
                    break;
                }
        
                for (std::size_t w = 1; w < n; ++w) {
                    std::size_t p = w * vec[i] * vec[j] * vec[k];
                    if (p <= n) {
                        res[p] = true;
                    } else {
                        break;
                    }
                }
            } 
        }
    }
    
    return res;
}

std::vector <bool>
solve_native (std::size_t n, std::vector <std::size_t>& primes) {
    std::vector <bool> res (n + 1, false);

    for (std::size_t i = 2; i <= n; ++i) {
        res[i] = is_3_prim (i, primes);
    }

    return res;
}

int main () {
    std::ios_base::sync_with_stdio (false);
    
    std::size_t N = 0;
    std::cin >> N;

    auto sieve = calc_sieve (N);
    auto primes = sieve2vec (sieve);
    //auto res = solve (N, primes);
    
    for (std::size_t n = 2; n <= N; n += 17 + n / 10) {
        DUMP (n);
        auto res = solve (n, primes);
        auto res_native = solve_native (n, primes);
    
        if (!std::equal (res.cbegin (), res.cend (), res_native.cbegin ())) {
            std::cout << "Fail: " << n << std::endl;
            return 0;
        }
        if (n % 1000 == 0) std::cout << n << std::endl;
    }

    return 0;
/*
    for (std::size_t i = 2; i < N; ++i) {
        if (res[i]) {
            std::cout << i << ' ';
        }
    }
*/
    std::cout << '\n';
}
