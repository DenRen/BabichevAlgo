#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iomanip>

struct coord_t {
    double x, y;

    coord_t (double x = 0, double y = 0) :
        x {x}, y {y}
    {}
};

struct point_t {
    coord_t coord;
    unsigned num;

    point_t () = default;

    point_t (double x, double y, unsigned num) :
        coord {x, y},
        num (num)
    {}
};

std::ostream&
operator << (std::ostream& os, const point_t& point) {
    return os << "{" << point.coord.x << ", " << point.coord.y << "}";
}

double
inline distance (const coord_t& lhs, const coord_t& rhs) {
    double dx = rhs.x - lhs.x;
    double dy = rhs.y - lhs.y;
    return std::sqrt (dx * dx + dy * dy);
}

double
inline distance (const point_t& lhs, const point_t& rhs) {
    return distance (lhs.coord, rhs.coord);
}

int main () {
    coord_t c1, c2;

    std::cin >> c1.x >> c1.y;
    std::cin >> c2.x >> c2.y;

    std::cout << distance (c1, c2) << std::endl;
}