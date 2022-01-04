#include <iostream>
#include <algorithm>
#include <vector>

template <typename T>
std::size_t
dance (T L, std::vector <T>& V) {
    std::sort (V.begin (), V.end ());
    std::size_t res = 0;

    for (auto pos = V.cbegin (); pos != V.cend (); ) {
        auto near = pos + 1;
        for (; near != V.cend () && *near - *pos <= 2 * L; ++near)
            ;
        pos = near;
        
        ++res;
    }

    return res;
} // dance (T L, std::vector <T>& V)

int main () {
    int L = 0;
    std::size_t N = 0;
    std::cin >> L >> N;

    std::vector <int> V (N);
    for (auto& v : V) {
        std::cin >> v;
    }

    std::cout << dance (L, V) << std::endl;
}