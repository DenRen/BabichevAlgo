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

void
func (const std::vector <double>& vec,
      std::size_t begin,
      std::size_t mid,
      std::size_t end,
      double dist)
{
    std::size_t prev = mid;   // Идекс последнего получившегося
    for (std::size_t j = begin; j < mid; ++j) {
        double y = vec[j];
        std::cout << y << ":\t";

        while (std::fabs (vec[prev] - y) > dist && prev < end) {
            ++prev;
        }

        for (std::size_t i = prev; i < end; --i) {
            if (std::fabs (vec[i] - y) <= dist) {
                std::cout << vec[i] << " ";
            } else {
                break;
            }
        }

        for (std::size_t i = prev + 1; i < end; ++i) {
            if (std::fabs (vec[i] - y) <= dist) {
                std::cout << vec[i] << " ";
            } else {
                prev = i - 1;
                break;
            }
        }

        std::cout << "\t\tprev: " << prev << "\n";
    }
}

int main () {
    std::vector <double> y1 = {1, 2, 5};
    std::vector <double> y2 = {1, 2, 3, 3.1, 3.2, 4.5, 5.5};
    double dist = 1;

    auto vec = y1;
    vec.insert (vec.end (), y2.cbegin (), y2.cend ());

    func (vec, 0, y1.size (), y1.size () + y2.size (), dist);
}
