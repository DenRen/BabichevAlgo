#include "solve.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    std::size_t size_cache = 0, num_req = 0;
    std::cin >> size_cache >> num_req;
    std::cout << solve (std::cin, size_cache, num_req) << '\n';
}