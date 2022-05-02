#include "../geoma/geoma.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    gtr::Triangle <long> triangle;
    std::cin >> triangle;
    do {
        gtr::Vector <long> point;
        std::cin >> point;
        if (std::cin.fail ()) {
            break;
        }

        auto in_triang = gtr::vector_in_triangle (triangle, point);
        std::cout << (in_triang ? "YES\n" : "NO\n");
    } while (true);
}