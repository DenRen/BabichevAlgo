#include "../numerus/numerus.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    constexpr std::size_t m = 1000000007;
    std::size_t n = 0, l = 0, r = 0;
    std::cin >> n >> l >> r;

    std::cout << nrs::sum_num_comb (n, l, r, m) << '\n';
}