#include "solve.hpp"

int main () {
    std::size_t N = 0;
    std::cin >> N;

    std::vector <std::pair <my_time_t <int>, my_time_t <int>>> intervals (N);
    for (auto& interval : intervals) {
        std::cin >> interval.first >> interval.second;
    }

    std::cout << busy_cells (intervals) << std::endl;
}