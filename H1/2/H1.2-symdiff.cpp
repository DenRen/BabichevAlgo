#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

#include "solve.hpp"

int main () {
    const std::size_t init_size = 128;
    std::vector <uint32_t> a, b;
    a.reserve (init_size);
    b.reserve (init_size);

    std::cin >> a >> b;

    std::cout << solve (a, b);
}
