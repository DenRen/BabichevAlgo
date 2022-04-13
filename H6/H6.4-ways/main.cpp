#include "solve.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    auto[lab, start, finish, num_iters] = read_lab (std::cin);
    std::cout << solve (lab, start, finish, num_iters) << '\n'; 
}
