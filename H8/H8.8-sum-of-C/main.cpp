#include "../numerus/numerus.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    constexpr std::size_t m = 1000000007;
    std::size_t n = 0, l = 0, r = 0;
    std::cin >> n >> l >> r;

    // if (n > 1ul << 22)  // Never, but on -O3 => fast_pow opt
    // for (int i = 0; i < 33; ++i) {
    //     volatile auto res = nrs::sum_num_comb (n, l, r, m);
    // }

    std::cout << nrs::sum_num_comb (n, l, r, m) << '\n';
}