#include "solve.hpp"

int main () {
    // std::cout << find_slow (8000) << std::endl;
    // return 0;

    std::ios_base::sync_with_stdio (false);

    std::size_t n = 0;
    std::cin >> n;
//    test (n);
    std::cout << solve2 (n) << std::endl;
}
