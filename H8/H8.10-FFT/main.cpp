#include "../numerus/numerus.hpp"
#include "solve.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    std::size_t n = 0, m = 0;

    std::cin >> n;
    std::vector <int> P (n + 1, 0);
    for (auto& p : P) {
        std::cin >> p;
    }
    
    std::cin >> m;
    std::vector <int> Q (m + 1, 0);
    for (auto& q : Q) {
        std::cin >> q;
    }

    auto R = nrs::fft::mult_poli_native (P, Q);
    std::cout << R.size () - 1;
    for (const auto& r : R) {
        std::cout << ' ' << r;
    }
    std::cout << '\n';
}