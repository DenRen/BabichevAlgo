#include "../geoma/geoma.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    gtr::LineSegment <double> ls1, ls2;
    std::cin >> ls1 >> ls2;
    std::cout << (gtr::is_intersect (ls1, ls2) ? "YES\n" : "NO\n");
}