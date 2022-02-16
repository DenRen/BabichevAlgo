#include "solve.hpp"

int main () {
    std::size_t M = 0, N = 0;
    std::cin >> M >> N;

    std::cout << solve_ref (M, N) << std::endl;
}