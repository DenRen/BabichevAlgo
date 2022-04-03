#include "solve.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    std::size_t N = 0, M = 0;
    std::cin >> N >> M;
    solve (std::cin, N, M);
}