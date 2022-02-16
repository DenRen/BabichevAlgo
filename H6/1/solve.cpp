#include "solve.hpp"

int solve (std::vector <int>& points) {
    const std::size_t size = points.size ();
    if (size <= 2) {
        int len = size <= 1 ? 0 : std::abs (points[1] - points[0]);
        return len;
    }

    // Here size >= 3
    std::sort (points.begin (), points.end ());
    int len = points[1] - points[0] + points[size-1] - points[size-2];

    for (std::size_t i = 1; i < size - 3; ++i) {
        int dist_first  = points[i + 1] - points[i];
        int dist_second = points[i + 2] - points[i + 1];

        if (dist_first < dist_second) {
            len += dist_first;
        } else {
            len += dist_second;
            ++i;
        }
    }

    return len;
}