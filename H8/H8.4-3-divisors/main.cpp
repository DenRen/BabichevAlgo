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
        
        while (++p < N && sieve[p] == false)
            ;
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

int main () {
    std::ios_base::sync_with_stdio (false);
    
    std::size_t n = 0;
    std::cin >> n;

    auto sieve = calc_sieve (n);
    auto vec = sieve2vec (sieve);
    std::vector <bool> res (n, false);
    
    auto num_primes = vec.size ();
    for (std::size_t i = 2; i < num_primes; ++i) {
        for (std::size_t j = i + 1; j < num_primes; ++j) {
            for (std::size_t k = j + 1; k < num_primes; ++k) {
                for (std::size_t w = 1; w < n; ++w) {
                    std::size_t p = w * vec[i] * vec[j] * vec[k];
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
            if (vec[i] * vec[j] * vec[j] > n) {
                break;
            }
        }
        if (vec[i] * vec[i] * vec[i] > n) {
            break;
        }
    }

    for (std::size_t i = 2; i < n; ++i) {
        if (res[i]) {
            std::cout << i << ' ';
        }
    }

    std::cout << '\n';
}
