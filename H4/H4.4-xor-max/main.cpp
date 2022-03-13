#include "solve.hpp"

int main () {
    unsigned N = 0;
    if (scanf ("%u", &N) != 1) {
        throw std::invalid_argument ("Read N failed");
    }
    solve (N, std::cin, std::cout);
}
