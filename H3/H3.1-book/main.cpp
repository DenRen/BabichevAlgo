#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cassert>

// K < N
std::size_t
solve_normal (std::istream& is, std::size_t K, std::size_t N) {
    assert (K < N);
    
    std::size_t max_vol = 0, vol_sum = 0;

    std::vector <std::size_t> vols (N);
    for (std::size_t i = 0; i < N; ++i) {
        is >> vols[i];
        max_vol = std::max (max_vol, vols[i]);
        vol_sum += vols[i];
    }

    std::size_t begin_vol = max_vol - 1;
    std::size_t end_vol = vol_sum;
    while (end_vol > begin_vol + 1) {
        std::size_t cur_vol = (begin_vol + end_vol) / 2;
        std::size_t count_vols = 1, sum_vols = 0;

        for (const auto& vol : vols) {
            if (sum_vols + vol <= cur_vol) {
                sum_vols += vol;
            } else {
                sum_vols = vol;
                ++count_vols;
            }
        }
        
        if (count_vols <= K) {
            end_vol = cur_vol;
        } else {
            begin_vol = cur_vol;
        }
    }

    return end_vol;
}

std::size_t
solve (std::istream& is, std::size_t K, std::size_t N) {
    if (K < N) {
        return solve_normal (is, K, N);
    }

    std::size_t M = 0, tmp = 0;
    while (N--) {
        is >> tmp;
        M = std::max (M, tmp);
    }
    
    return M;
}

int main () {
    std::size_t K = 0, N = 0;
    std::cin >> K >> N;

    std::size_t M = solve (std::cin, K, N);
    std::cout << M << std::endl;
}
