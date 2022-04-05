#include "solve.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);
    
    int N = 0;
    std::cin >> N;
    std::cout << solve (N) << std::endl;
}