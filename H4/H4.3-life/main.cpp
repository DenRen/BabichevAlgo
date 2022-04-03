#include "solve.hpp"

int main () {
    std::ios::sync_with_stdio (false);

    std::size_t N = 0, M = 0, L = 0, K = 0;
    std::cin >> N >> M >> L >> K;
    
    // native_solve (N, M, L, K);
    solve (N, M, L, K);
}