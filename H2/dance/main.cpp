#include "solve.hpp"

int main () {
    int L = 0;
    std::size_t N = 0;
    std::cin >> L >> N;

    std::vector <int> V (N);
    for (auto& v : V) {
        std::cin >> v;
    }

    std::cout << dance (L, V) << std::endl;
}