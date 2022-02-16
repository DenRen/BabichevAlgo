#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <map>

#include "other_func.hpp"

int solve (std::vector <int>& points);

template <typename T>
std::vector <T>
get_vector (std::istream& is = std::cin) {
    std::size_t N = 0;
    is >> N;

    std::vector <T> vec;
    vec.reserve (N);
    while (N--) {
        T value;
        is >> value;
        vec.emplace_back (std::move (value));
    }

    return vec;
}