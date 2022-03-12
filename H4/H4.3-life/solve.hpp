#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <map>
#include <queue>
#include <set>
#include <iomanip>
#include <stack>

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define dump(obj)
#endif

void
print_map (const std::vector <std::vector <bool>>& map) {
    for (const auto& line : map) {
        for (auto p : line) {
            if (p) {
                std::cout << "x ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

unsigned
sum_around (const std::vector <std::vector <bool>>& map,
            unsigned x, unsigned y)
{
    unsigned sum = 0;

    auto left_bound  = x == 0 ? 0 : x - 1;
    auto down_bound  = y == 0 ? 0 : y - 1;
    auto right_bound = x == (map[0].size () - 1) ? x : x + 1;
    auto up_bound    = y == (map.size ()    - 1) ? y : y + 1;

    for (auto j = down_bound; j <= up_bound; ++j) {
        for (auto i = left_bound; i <= right_bound; ++i) {
            sum += map[j][i];
        }
    }

    return sum - map[y][x];
}

void
native_solve (std::size_t N, std::size_t M, std::size_t L, std::size_t K) {
    std::vector <std::vector <bool>> map (M);
    for (auto& line : map) {
        line = std::vector <bool> (N);
    }

    for (std::size_t i = 0; i < L; ++i) {
        unsigned x = 0, y = 0;
        std::cin >> x >> y;
        map[y][x] = true;
    }

    // print_map (map);
    // std::cout << "\n";

    auto tmp = map;
    for (std::size_t i = 0; i < K; ++i) {
        for (std::size_t y = 0; y < M; ++y) {
            for (std::size_t x = 0; x < N; ++x) {
                unsigned sum = sum_around (map, x, y);
                // std::cout << sum << " ";
                if (map[y][x]) {
                    tmp[y][x] = (sum == 2) || (sum == 3);
                } else {
                    tmp[y][x] = sum == 3;
                }
            }
            // std::cout << "\n";
        }
        std::swap (tmp, map);

        // print_map (map);
        // std::cout << "\n";
    }

    for (std::size_t x = 0; x < N; ++x) {
        for (std::size_t y = 0; y < M; ++y) {
            if (map[y][x]) {
                std::cout << x << " " << y << "\n";
            }
        }
    }
}