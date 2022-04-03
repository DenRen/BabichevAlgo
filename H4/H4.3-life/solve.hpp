#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <map>
#include <queue>
#include <set>
#include <iomanip>
#include <stack>
#include <fstream>
#include <iterator>

// g++ -DHOST -std=c++17 main.cpp

// #define NDEBUG
// #define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define DUMP(obj)
#endif

void
print_map (const std::vector <std::vector <bool>>& map) {
    for (const auto& line : map) {
        for (auto p : line) {
            if (p) {
                std::cout << "x ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

unsigned
sum_around (const std::vector <std::vector <bool>>& map,
            unsigned x, unsigned y)
{
    unsigned sum = 0;

    auto left_bound  = x == 0 ? 0 : x - 1;
    auto down_bound  = y == 0 ? 0 : y - 1;
    auto right_bound = x == (map[0].size () - 1) ? x : x + 1;
    auto up_bound    = y == (map.size ()    - 1) ? y : y + 1;

    for (auto j = down_bound; j <= up_bound; ++j) {
        for (auto i = left_bound; i <= right_bound; ++i) {
            sum += map[j][i];
        }
    }

    return sum - map[y][x];
}

void
native_solve (std::size_t N, std::size_t M, std::size_t L, std::size_t K) {
    std::vector <std::vector <bool>> map (M);
    for (auto& line : map) {
        line = std::vector <bool> (N);
    }

    for (std::size_t i = 0; i < L; ++i) {
        unsigned x = 0, y = 0;
        std::cin >> x >> y;
        map[y][x] = true;
    }

    auto tmp = map;
    for (std::size_t i = 0; i < K; ++i) {
        for (std::size_t y = 0; y < M; ++y) {
            for (std::size_t x = 0; x < N; ++x) {
                unsigned sum = sum_around (map, x, y);
                if (map[y][x]) {
                    tmp[y][x] = (sum == 2) || (sum == 3);
                } else {
                    tmp[y][x] = sum == 3;
                }
            }
        }
        std::swap (tmp, map);
    }

    for (std::size_t x = 0; x < N; ++x) {
        for (std::size_t y = 0; y < M; ++y) {
            if (map[y][x]) {
                std::cout << x << " " << y << "\n";
            }
        }
    }
}

struct coord_t {
    int x = 0, y = 0;

    coord_t () = default;
    coord_t (int x, int y) :
        x (x), y (y)
    {}

    bool
    operator < (const coord_t& rhs) const noexcept {
        return x < rhs.x || (x == rhs.x && y < rhs.y);
    }

    bool
    operator == (const coord_t& rhs) const noexcept {
        return x == rhs.x && y == rhs.y;
    }
};

std::ostream&
operator << (std::ostream& os, const coord_t& coord) {
    return os << '{' << coord.x << ", " << coord.y << '}';
}

std::set <coord_t>
read_points (std::size_t num, std::istream& is = std::cin) {
    std::set <coord_t> points;

    for (std::size_t i = 0; i < num; ++i) {
        coord_t coord;
        std::cin >> coord.x >> coord.y;
        points.insert (coord);
    }

    return points;
}

bool
is_y_belong (int y1, int y2) {
    return std::abs (y1 - y2) <= 1;
}

struct area_t {
    bool life[5][5] = {};

    // -2 <= x <= 2
    // -2 <= y <= 2


    bool
    get (int x, int y) const {
        return life[y + 2][x + 2];
    }

    bool&
    get (int x, int y) {
        return life[y + 2][x + 2];
    }

    void
    set (int x, int y) {
        get (x, y) = true;
    }

    // without center
    int
    get_sum (int x0, int y0) const {
        int sum = 0;
        for (int x = x0 - 1; x <= x0 + 1; ++x) {
            for (int y = y0 - 1; y <= y0 + 1; ++y) {
                sum += get (x, y);
            }
        }

        return sum - get (x0, y0);
    }
};

std::ostream&
operator << (std::ostream& os, const area_t& area) {
    for (int y = 2; y >= -2; --y) {
        os << area.get (-2, y);
        for (int x = -1; x <= 2; ++x) {
            os << ' ' << area.get (x, y);
        }
        os << '\n';
    }

    return os;
}

std::vector <coord_t>
get_vector (const std::set <coord_t>& set) {
    std::vector <coord_t> points (set.size ());
    std::size_t i = 0;
    for (const auto& p : set) {
        points[i++] = p;
    }

    return points;
}

std::vector <bool>
get_life_map (const std::set <coord_t>& points, std::size_t N, std::size_t M) {
    std::vector <bool> map (N * M);
    for (const auto& p : points) {
        map[p.x + p.y * N] = true;
    }

    return map;
}

void
print_life_map (const std::vector <bool>& map, std::size_t N, std::size_t M) {
    for (int y = M - 1; y >= 0; --y) {
        for (int x = 0; x < N; ++x) {
            std::cout << (map[x + y * N] ? 'x' : '.');
        }
        std::cout << std::endl;
    }
}

area_t
fill_area (std::set <coord_t>& points, std::set <coord_t>::const_iterator it_center) {
    auto x0 = it_center->x;
    auto y0 = it_center->y;

    area_t area;
    auto begin = points.cbegin (), end = points.cend ();
    for (auto x = x0 - 2; x <= x0 + 2; ++x) {
        auto it = points.lower_bound ({x, y0 - 2});

        for (; it != end && it->x == x; ++it) {
            if (std::abs (it->y - y0) > 2) {
                break;
            }
            area.set (x - x0, it->y - y0);
        }
    }

    return area;
}

void
evoluate_area (const area_t& area,
               const coord_t& coord, 
               std::vector <coord_t>& new_points,
               std::vector <coord_t>& killed_points) {
    for (int x0 = -1; x0 <= 1; ++x0) {
        for (int y0 = -1; y0 <= 1; ++y0) {

            auto x = coord.x + x0, y = coord.y + y0;
            int sum = area.get_sum (x0, y0);
            if (area.get (x0, y0)) {
                if (sum < 2 || sum > 3) {
                    killed_points.push_back ({x, y});
                }
            } else {
                if (sum == 3) {
                    new_points.push_back ({x, y});
                }
            }
        }
    }
}

bool
is_belong_map (const coord_t& coord, std::size_t N, std::size_t M) {
    return coord.x >= 0 && coord.x < N &&
           coord.y >= 0 && coord.y < M;
}

void
evoluate_points (std::set <coord_t>& points,
                 std::size_t N, std::size_t M, std::size_t K) {
    std::vector <coord_t> new_points, killed_points;
    for (std::size_t num_iter = 0; num_iter < K; ++num_iter) {
        for (auto it = points.cbegin (); it != points.cend (); ++it) {
            area_t area = fill_area (points, it);
            evoluate_area (area, *it, new_points, killed_points);
        }

        for (const auto& point : new_points) {
            if (is_belong_map (point, N, M)) {
                points.insert (point);
            }
        }
        new_points.clear ();

        for (const auto& point : killed_points) {
            if (is_belong_map (point, N, M)) {
                points.erase (point);
            }
        }
        killed_points.clear ();
    }
}

void
solve (std::size_t N, std::size_t M, std::size_t L, std::size_t K) {
    auto points = read_points (L);
    evoluate_points (points, N, M, K);

    for (const auto& point : points) {
        std::cout << point.x << ' ' << point.y << '\n';
    }

    // auto points_vec = get_life_map (points, N, M);
    // print_life_map (points_vec, N, M);
}