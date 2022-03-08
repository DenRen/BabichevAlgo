#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iomanip>

// g++ -DHOST -std=c++17 -O3 main.cpp

// #define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define dump(obj)
#endif

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
    return os << point.coord.x << " " << point.coord.y << "\n";
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

std::pair <std::size_t, std::size_t>
calc_min_l_max_r_index (const std::vector <point_t>& points_sort_x,
                        std::size_t begin,
                        std::size_t end,
                        double dist)
{
    const auto size = end - begin;

    auto pointx_comparator = [] (const point_t& lhs, const point_t& rhs) {
        return lhs.coord.x < rhs.coord.x;
    };
    const auto begin_it = points_sort_x.cbegin () + begin;
    const auto helf_it  = begin_it + size / 2;
    const auto end_it   = begin_it + size;
    std::size_t min_l_index = std::lower_bound (
        begin_it, helf_it,
        point_t {points_sort_x[begin + size / 2 - 1].coord.x - dist, 0, 0},
        pointx_comparator

    ) - points_sort_x.begin ();
    std::size_t max_r_index = std::upper_bound (
        helf_it, end_it,
        point_t {points_sort_x[begin + size / 2 - 1].coord.x + dist, 0, 0},
        pointx_comparator
    ) - points_sort_x.begin ();

    return {min_l_index, max_r_index};
}

std::tuple <double, std::size_t, std::size_t>
min_dist_2_part (const std::vector <point_t>& points_sort_y,
                 std::size_t begin,
                 std::size_t middle,
                 std::size_t end)
{
    auto calc_dist = [&] (std::size_t index_sort_y_l, std::size_t index_sort_y_r) {
        return distance (points_sort_y[index_sort_y_l], points_sort_y[index_sort_y_r]);
    };

    std::pair <std::size_t, std::size_t> min_dist_indexes = {begin, middle};
    double min_dist = calc_dist (min_dist_indexes.first, min_dist_indexes.second);

    auto update_min_dist = [&] (std::size_t index_l, std::size_t index_r) {
        // Точка index_r попала в диапазон точки index_l
        double new_min_dist = calc_dist (index_l, index_r);

        if (new_min_dist < min_dist) {
            min_dist = new_min_dist;
            min_dist_indexes = {index_l, index_r};
        } else if (new_min_dist == min_dist) {
            std::size_t real_index_cur_l = points_sort_y[min_dist_indexes.first].num;
            std::size_t real_index_new_l = points_sort_y[index_l].num;

            if (real_index_new_l < real_index_cur_l) {
                min_dist_indexes = {index_l, index_r};
            } else if (real_index_new_l == real_index_cur_l) {
                std::size_t real_index_cur_r = points_sort_y[min_dist_indexes.second].num;
                std::size_t real_index_new_r = points_sort_y[index_r].num;

                if (real_index_new_r < real_index_cur_r) {
                    min_dist_indexes.second = index_r;
                }
            }
        }
    };

    auto get_y = [&] (std::size_t index_sort_y) {
        return points_sort_y[index_sort_y].coord.y;
    };

    // for (std::size_t j = middle; j < end; ++j) {
    //     update_min_dist (i, j);
    // }
    // continue;

    std::size_t up_r = end - 1;   // Идекс последнего получившегося
    for (std::size_t i = middle - 1; i + 1 > begin ; --i) {
        double y = get_y (i);

        double up_bound = y, down_bound = y - min_dist;
        while (get_y (up_r) > up_bound && up_r > middle) {
            --up_r;
        }

        std::size_t save_up_r = up_r;

        while (get_y (up_r) >= down_bound && up_r > middle) {
            update_min_dist (i, up_r--);
        }
        update_min_dist (i, up_r);

        up_r = save_up_r;
    }

    std::size_t up_l = middle - 1;
    for (std::size_t i = end - 1; i + 1 > middle ; --i) {
        double y = get_y (i);

        double up_bound = y, down_bound = y - min_dist;
        while (get_y (up_l) > up_bound && up_l > begin) {
            --up_l;
        }

        std::size_t save_up_l = up_l;

        while (get_y (up_l) >= down_bound && up_l > begin) {
            update_min_dist (up_l--, i);
        }
        update_min_dist (up_l, i);

        up_l = save_up_l;
    }

    return {min_dist, points_sort_y [min_dist_indexes.first].num,
                      points_sort_y [min_dist_indexes.second].num};
}

void
merge_sort_y (std::vector <point_t>& points_sort_y,
              std::size_t begin,
              std::size_t middle,
              std::size_t end)
{
    const auto size = end - begin;
    std::vector <point_t> tmp (size);

    auto begin_it  = points_sort_y.begin () + begin;
    auto middle_it = points_sort_y.begin () + middle;
    auto end_it    = points_sort_y.begin () + end;

    std::merge (begin_it, middle_it,
                middle_it, end_it,
                tmp.begin (),
                [] (const point_t& lhs, const point_t& rhs) {
                    return lhs.coord.y < rhs.coord.y;
                });

    std::copy (tmp.begin (), tmp.end (), begin_it);
}

std::tuple <double, std::size_t, std::size_t>
min_dist (const std::vector <point_t>& points_sort_x,
          std::vector <point_t>& points_sort_y,
          std::size_t begin,
          std::size_t end)
{
    const auto size = end - begin;
    if (size <= 2) {
        if (size == 2) {
            auto& point_l = points_sort_y[begin];
            auto& point_r = points_sort_y[begin + 1];
            if (point_l.coord.y > point_r.coord.y) {
                std::swap (point_l, point_r);
            }

            return {distance (points_sort_x[begin], points_sort_x[begin + 1]),
                    points_sort_x[begin].num,
                    points_sort_x[begin + 1].num};
        }

        return {1e200, -1, -1};
    }

    std::size_t middle = begin + size / 2;

    const auto[min_dist_l, index_l_l, index_l_r] =
        min_dist (points_sort_x, points_sort_y, begin, middle);
    const auto[min_dist_r, index_r_l, index_r_r] =
        min_dist (points_sort_x, points_sort_y, middle, end);

    double dist = std::min (min_dist_l, min_dist_r);

    const auto[min_l_index, max_r_index] =
       calc_min_l_max_r_index (points_sort_x, begin, end, dist);

    const auto[min_dist_between, index_b_l, index_b_r] =
        min_dist_2_part (points_sort_y, begin, middle, end);

    merge_sort_y (points_sort_y, begin, middle, end);

    dist = std::min (dist, min_dist_between);

    if (dist == min_dist_l) {
        return {min_dist_l, index_l_l, index_l_r};
    } else if (dist == min_dist_between) {
        return {min_dist_between, index_b_l, index_b_r};
    } else {
        return {min_dist_r, index_r_l, index_r_r};
    }
}

std::pair <std::size_t, std::size_t>
get_sorted_indexes (std::size_t index_l,
                    std::size_t index_r)
{
    if (index_l < index_r) {
        return {index_l, index_r};
    } else {
        return {index_r, index_l};
    }
}

std::tuple <double, std::size_t, std::size_t>
solve (std::vector <point_t>& points)
{
    std::sort (points.begin (), points.end (),
        [] (const point_t& lhs, const point_t& rhs) {
            return lhs.coord.x < rhs.coord.x;
        });

    auto& points_sort_x = points;
    auto points_sort_y = points_sort_x;

    auto[dist, index_sort_x_l, index_sort_x_r] =
        min_dist (points_sort_x, points_sort_y, 0, points.size ());

    auto[index_l, index_r] = get_sorted_indexes (index_sort_x_l, index_sort_x_r);

    return {dist, index_l, index_r};
}

std::ostream&
print_answer (std::tuple <double, std::size_t, std::size_t> answer,
              std::ostream& os = std::cout)
{
    auto&[dist, index1, index2] = answer;
    return os << std::fixed << std::setprecision (10) << dist
              << " " << index1 + 1
              << " " << index2 + 1 << "\n";
}

std::vector <point_t>
read_points (std::istream& is = std::cin)
{
    std::size_t N = 0;
    is >> N;
    if (N == 0) {
        throw std::invalid_argument ("N == 0");
    } else if (N == 1) {
        print_answer ({0, 0, 0});
        exit (0);
    }

    std::vector <point_t> points (N);
    std::size_t i = 0;
    for (auto&[coord, num] : points) {
        is >> coord.x >> coord.y;
        if (std::cin.fail ()) {
            throw std::runtime_error ("std::cin");
        }
        num = i++;
    }

    return points;
}

int main () {
    auto points = read_points ();
    auto answer = solve (points);
    print_answer (answer);
}
