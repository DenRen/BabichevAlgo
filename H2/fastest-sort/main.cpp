#include <iostream>
#include "solve.hpp"

int main () {
    data_t N = 0, K = 0, M = 0, L = 0;
    std::cin >> N >> K >> M >> L;

    data_t result = solve_from_radix (N, K, M, L);
    std::cout << result << std::endl;
}