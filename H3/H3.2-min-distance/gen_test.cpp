#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <thread>

#include "../../libs/other_func.hpp"
#include "solve.hpp"

std::tuple <double, std::size_t, std::size_t>
native_solve (const std::vector <point_t>& points)
{
    double min_dist = 1e20;
    std::size_t index_l = -1, index_r = -1;

    const auto size = points.size ();
    for (std::size_t i = 0; i < size; ++i) {
        for (std::size_t j = i + 1; j < size; ++j) {
            double dist = distance (points[i], points[j]);

            if (dist < min_dist) {
                min_dist = dist;
                index_l = i;
                index_r = j;
            } else if (dist == min_dist) {
                if (i < index_l) {
                    index_l = i;
                    index_r = j;
                } else if (i == index_l) {
                    if (j < index_r) {
                        index_r = j;
                    }
                }
            }
        }
    }

    return {min_dist, index_l, index_r};
}

void begin_test (std::size_t num_tests) {
    seclib::RandomGenerator rand;

    for (std::size_t num_test = 0; num_test < num_tests; ++num_test) {
        std::size_t N = rand.get_rand_val <std::size_t> (30, 100);
        std::vector <point_t> points (N);

        for (unsigned i = 0; i < N; ++i) {
            point_t point {(double) (rand.get_rand_val <unsigned> () % 1000000),
                           (double) (rand.get_rand_val <unsigned> () % 1000000), i};
            points[i] = point;
        }

        auto[ref_dist, index_sort_x_l, index_sort_x_r] = native_solve (points);
        auto[ref_index_l, ref_index_r] = get_sorted_indexes (index_sort_x_l, index_sort_x_r);

        auto copy_points = points;
        auto[dist, index_l, index_r] = solve (copy_points);

        if (dist != ref_dist) {
            std::cout << "failed!\n";
            print_answer ({ref_dist, ref_index_l, ref_index_r});
            print_answer ({dist, index_l, index_r});

            std::cout << points << std::endl;
            exit (0);
        } else {
            // std::cout << "good!\t" << num_test << "\\" << num_tests << "\n";
        }
    }
}

int main () {
    std::size_t num_tests = 10000000;
    std::size_t num_threads = std::thread::hardware_concurrency ();

    std::vector <std::thread> threads;
    threads.reserve (num_threads);
    for (std::size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back (begin_test, num_tests / num_threads);
    }

    for (auto& thread : threads) {
        thread.join ();
    }
}