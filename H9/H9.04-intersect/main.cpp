#include "../geoma/geoma.hpp"

bool
solve (gtr::Vector <double> p1,
       gtr::Vector <double> q1,
       gtr::Vector <double> p2,
       gtr::Vector <double> q2) {
    
}

int main () {
    std::ios_base::sync_with_stdio (false);

    gtr::Vector <double> p1, q1, p2, q2;
    std::cin >> p1 >> q1 >> p2 >> q2;

    std::cout << (solve (p1, q1, p2, q2) ? "YES\n" : "NO\n");
}