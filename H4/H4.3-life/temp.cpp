#include "solve.hpp"

int main () {
    std::size_t N = 0, M = 0, L = 0, K = 0;
    std::cin >> N >> M >> L >> K;

    auto points = read_points (L);
    
    auto life_map = get_life_map (points, N, M);
    print_life_map (life_map, N, M);

    std::cout << *points.begin () << std::endl;
    area_t area = fill_area (points, points.begin ());
    std::cout << area << std::endl;
}