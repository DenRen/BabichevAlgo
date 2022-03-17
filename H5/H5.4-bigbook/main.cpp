#include "solve.hpp"

int main () {
    const auto Mb = 1024 * 1024;
    const std::size_t max_ram = 400 * Mb;

    std::ios::sync_with_stdio (false);
    std::size_t N = 0;
    std::cin >> N;

    solve (N, std::cout, max_ram);
}