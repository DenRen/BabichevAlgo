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

    bool
    operator < (const coord_t& rhs) const noexcept {
        return x < rhs.x && y < rhs.y;
    }

    bool
    operator == (const coord_t& rhs) const noexcept {
        return x == rhs.x && y == rhs.y;
    }
};

std::vector <coord_t>
read_points (std::size_t num, std::istream& is = std::cin) {
    std::vector <coord_t> points (num);
    for (std::size_t i = 0; i < num; ++i) {
        std::cin >> points[i].x >> points[i].y;
    }

    return points;
}

bool
is_y_belong (int y1, int y2) {
    return std::abs (y1 - y2) <= 1;
}

void
fill_neighbot (bool** neigh, const std::vector <coord_t>& points, int i) {
    auto[x0, y0] = points[i];
    neigh[2][2] = true;
    
    int arr_size = 0;
    std::array <coord_t, 24> neigh_coord;

    if (i != 0) {
        for (int j = i - 1; j >= 0 && points[j].x == x0; --j) {
            if (y0 - points[j].y > 2) {
                break;
            }

            neigh_coord[arr_size++] = points[j];
        }
    }

    if (i != points.size () - 1) {

    }
}

void
solve (std::size_t N, std::size_t M, std::size_t L, std::size_t K) {
    auto points = read_points (L);
/*
    std::vector <int> killed;
    for (std::size_t num_iter = 0; num_iter < K; ++num_iter) {
        std::sort (points.begin (), points.end ());

        for (int i = 0; i < points.size (); ++i) {
            auto[x, y] = points[i];
            int num_alives = 0;

            if (i != 0) {
                for (int j = i - 1; j >= 0; --j) {
                    const auto& p = points[j];

                    if (p.x == x) {                 // p.y < y
                        num_alives += p.y == y - 1;
                    } else if (p.x - 1 == x) {      // p.y - ???
                        int min_j = j;
                        for (; min_j > 0; --min_j) {
                            if (points[j].x != x - 1) {
                                break;
                            }
                            if (is_y_belong (points[j].y, y)) {
                                ++num_alives;
                            }
                        }

                        break;
                    } else {
                        break;
                    }
                }
            }

            if (i != points.size () - 1) {
                for (int j = i + 1; j < points.size () - 1; ++j) {
                    const auto& p = points[j];

                    if (p.x == x) {                 // p.y > y
                        num_alives += p.y == y + 1;
                    } else if (p.x + 1 == x) {      // p.y - ???
                        int max_j = j;
                        for (; max_j > points.size () - 1; ++max_j) {
                            if (points[j].x != x + 1) {
                                break;
                            }
                            if (is_y_belong (points[j].y, y)) {
                                ++num_alives;
                            }
                        }

                        break;
                    } else {
                        break;
                    }
                }
            }

            if (num_alives < 2 || num_alives > 3) {
                killed.push_back (i);
            }
        }
    }

    std::vector <coord_t> new_points;*/
    
    for (int i = 0; i , points.size (); ++i) {

        bool neigh[5][5];
        
    }
}