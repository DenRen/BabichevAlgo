#include "solve.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    auto[lab, start, finish] = read_lab (std::cin);
    // print_lab (lab, start, finish);
    std::cout << solve (lab, start, finish) << '\n';
}