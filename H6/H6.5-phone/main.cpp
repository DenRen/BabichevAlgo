#include "solve.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

std::size_t
solve (std::size_t n, std::size_t k) {
    std::vector <std::vector <std::size_t>> S (k);
    for (auto& s : S) {
        s.resize (n + 1);
        std::fill (s.begin (), s.end (), -1);
    }

    // S[k][i] - num of S in i + 1 floors, k + 1 phones
    // Fill S by k == 1
    // S[0][n] = n - 1 => S[0] = 0, 0, 1, 2, 3, 4, ...
    S[0][0] = 0;
    for (int num_floor = 1; num_floor <= n; ++num_floor) {
        S[0][num_floor] = num_floor - 1;
    }

    for (int cur_k = 2; cur_k <= k; ++cur_k) {
        S[cur_k - 1][0] = 0;
        S[cur_k - 1][1] = 0;
        for (int num_floor = 2; num_floor <= n; ++num_floor) {
            std::size_t min_s = -1;
            for (int i = 1; i <= num_floor; ++i) {
                std::size_t s = 1 + std::max (
                    S[cur_k - 1][num_floor - i], S[cur_k - 2][i]
                );
                
                if (s < min_s) {
                    min_s = s;
                }
            }
            
            S[cur_k - 1][num_floor] = min_s;
        }
    }

    return S[k - 1][n];
}

int main () {
    // 1 <= 100'000 -> (1 << 17) > 100'000

    std::size_t n = 0, k = 0;
    std::cin >> n >> k;
    
    long res = -1;
    if (n == 0 || n == 1) {
        res = 0;
    } else if (k == 0) {
        res = -1;
    } else {
        res = solve (n, k);
    }

    std::cout << res << '\n';

    return 0;
}
