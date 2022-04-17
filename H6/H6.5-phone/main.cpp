#include "solve.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

std::size_t
solve (std::size_t n, std::size_t k) {
    std::vector <std::size_t> S_prev (n + 1), S (n + 1);

    // S[k][i] - num of S in i + 1 floors, k + 1 phones
    // Fill S by k == 1
    // S[0][n] = n - 1 => S[0] = 0, 0, 1, 2, 3, 4, ...
    S_prev[0] = 0;
    for (int num_floor = 1; num_floor <= n; ++num_floor) {
        S_prev[num_floor] = num_floor - 1;
    }
    DUMP (S_prev);
    auto* S_prev_ptr = &S_prev, *S_ptr = &S;

    for (int cur_k = 2; cur_k <= k; ++cur_k) {
        auto& cur_arr = *S_ptr;
        auto& prev_arr = *S_prev_ptr;

        cur_arr[0] = 0;
        cur_arr[1] = 0;
        for (int num_floor = 2; num_floor <= n; ++num_floor) {
            std::size_t min_s = -1;
            for (int i = 1; i <= num_floor; ++i) {
                std::size_t s = 1 + std::max (
                    cur_arr[num_floor - i], prev_arr[i]
                );
                
                if (s < min_s) {
                    min_s = s;
                }
            }
            
            cur_arr[num_floor] = min_s;
        }
        DUMP (S);
        std::swap (S_ptr, S_prev_ptr);
    }

    return (*S_prev_ptr)[n];
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
