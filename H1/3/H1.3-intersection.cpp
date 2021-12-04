#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

#include "other_func.hpp"
#include "solve.hpp"

int main () {
    std::size_t size_a = 0, size_b = 0;
    std::cin >> size_a >> size_b;

    std::vector <uint32_t> a, b;
    a.reserve (size_a);
    b.reserve (size_b);

    decltype (a)::value_type value = 0;
    for (std::size_t i = 0; i < size_a; ++i) {
        std::cin >> value;
        a.push_back (value);
    }
    for (std::size_t i = 0; i < size_b; ++i) {
        std::cin >> value;
        b.push_back (value);
    }

    std::cout << solve (a, b);
}
