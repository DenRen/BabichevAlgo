#include "solve.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    long a = 0, b = 0,c = 0;
    std::cin >> a >> b >> c;
    
    auto [is_no, res] = solve_diaf (a, b, c);
    
    if (is_no) {
        std::cout << "No\n";
    } else {
        const auto& x = res.first, &y = res.second;
        std::cout << x << ' ' << y << '\n';
    }
}
