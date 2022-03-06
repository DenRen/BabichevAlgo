#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <cmath>

// g++ -DHOST -std=c++17 main.cpp

#define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#endif

struct coord_t {
    double x, y;

    coord_t (double x = 0, double y = 0) :
        x (x), y (y)
    {}
};

struct point_t {
    coord_t coord;
    unsigned num;

    point_t () = default;

    point_t (double x, double y, unsigned num) :
        coord (x, y),
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
    const auto helf_it = begin_it + size / 2;
    const auto end_it = begin_it + end;
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
    std::pair <std::size_t, std::size_t> min_dist_indexes = {begin, middle};
    double min_dist = distance (points_sort_y[min_dist_indexes.first],
                                points_sort_y[min_dist_indexes.second]);

    std::size_t prev = middle;   // Идекс последнего получившегося
    for (std::size_t j = begin; j < middle; ++j) {
        double y = points_sort_y[j].coord.y;

        for (std::size_t i = prev; i + 1 > middle && i < end; --i) {
            if (std::fabs (points_sort_y[i].coord.y - y) > min_dist) {
                break;
            }

            // Точка попала в диапазон
            min_dist = std::min (min_dist, distance (points_sort_y[j], points_sort_y[i]));
            min_dist_indexes = {j, i};
        }

        for (std::size_t i = prev + 1; i < end; ++i) {
            if (std::fabs (points_sort_y[i].coord.y - y) > min_dist) {
                prev = i - 1;
                break;
            }

            // Точка попала в диапазон
            min_dist = std::min (min_dist, distance (points_sort_y[j], points_sort_y[i]));
            min_dist_indexes = {j, i};
        }
    }

    return {min_dist, min_dist_indexes.first, min_dist_indexes.second};
}

std::tuple <double, std::size_t, std::size_t>
min_dist (const std::vector <point_t>& points_sort_x,
          std::vector <point_t>& points_sort_y,
          std::size_t begin,
          std::size_t end)
{
    std::cout << begin << "->" << end << std::endl;
    const auto size = end - begin;
    if (size <= 2) {
        if (size == 2) {
            auto& point_l = points_sort_y[begin];
            auto& point_r = points_sort_y[begin + 1];
            if (point_l.coord.y > point_r.coord.y) {
                std::swap (point_l, point_r);
            }

            return {distance (points_sort_x[begin], points_sort_x[begin + 1]), begin, begin + 1};
        }

        return {1e200, -1, -1};
    }

    auto[min_dist_l, index_l_l, index_l_r] = min_dist (points_sort_x, points_sort_y, begin, begin + size / 2);
    auto[min_dist_r, index_r_l, index_r_r] = min_dist (points_sort_x, points_sort_y, begin + size / 2 , end);

    double dist = std::min (min_dist_l, min_dist_r);

    std::cout << min_dist_l << " " << min_dist_r << std::endl;

    auto[min_l_index, max_r_index] = calc_min_l_max_r_index (points_sort_x, begin, end, dist);

    auto[min_dist_between, index_b_l, index_b_r] =
        min_dist_2_part (points_sort_y, begin, begin + size / 2, end);

    if (dist < min_dist_between) {
        if (min_dist_l < min_dist_r) {
            return {min_dist_l, index_l_l, index_l_r};
        } else {
            return {min_dist_r, index_r_l, index_r_r};
        }
    } else {
        return {min_dist_between, index_b_l, index_b_r};
    }
}

std::pair <std::size_t, std::size_t>
get_sorted_indexes (const std::vector <point_t>& points_sort,
                    std::size_t index_l,
                    std::size_t index_r)
{
    index_l = points_sort[index_l].num;
    index_r = points_sort[index_r].num;

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

    auto[index_l, index_r] = get_sorted_indexes (points_sort_x, index_sort_x_l, index_sort_x_r);

    return {dist, index_l, index_r};
}

int main () {
    std::size_t N = 0;
    std::cin >> N;

    std::vector <point_t> points (N);
    unsigned i = 0;
    for (auto&[coord, num] : points) {
        std::cin >> coord.x >> coord.y;
        if (std::cin.fail ()) {
            throw std::runtime_error ("std::cin");
        }
        num = i++;
    }

    dump (points);

    auto[dist, index1, index2] = solve (points);
    std::cout << dist << " " << index1 + 1 << " " << index2 + 1 << "\n";
}
