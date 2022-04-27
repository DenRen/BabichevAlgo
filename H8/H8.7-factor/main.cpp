#include "../numerus/numerus.hpp"
#include "solve.hpp"

int main () {
#if 1
//     std::cout << find_slow (8000) << std::endl;
//     return 0;

#else
    std::size_t n = 0;
    std::cin >> n;

    nrs::factorizer factorizer;
    std::cout << factorizer (n) << std::endl;
#endif
}
