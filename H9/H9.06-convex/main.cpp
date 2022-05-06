#include <iomanip>
#include "../geoma/geoma.hpp"

template <typename T>
T
solve (std::vector <gtr::Vector <T>>& ps) {
    auto convex_hull = gtr::build_convex_hull_graham (ps);
    auto p = gtr::perimeter (convex_hull);

    return p;
}

int main () {
    std::ios_base::sync_with_stdio (false);

    std::size_t n = 0;
    std::cin >> n;

    std::vector <gtr::Vector <double>> ps (n);
    for (auto& p : ps) {
        std::cin >> p;
    }

    std::cout << std::setprecision (3) << solve (ps) << '\n';
}