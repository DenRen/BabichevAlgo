#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <map>

#include "other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -lpthread -O3 && clear && ./a.out

void print_subsets (const std::vector <std::tuple <char, int>>& set,
           std::vector <std::tuple <char, int>>::const_iterator pos,
           std::string cur = "")
{
    if (pos != set.cend ()) {
        const auto& pair = *pos++;

        auto symbol = std::get <char> (pair);
        auto repeat = std::get <int> (pair);

        for (int i = 0; i <= repeat; ++i) {
            print_subsets (set, pos, cur);
            cur += symbol;
        }
    } else {
        std::cout << cur << std::endl;
    }
}

void solve (std::string input_set) {
    std::map <char, int> _set;
    for (const auto& item : input_set) {
        ++_set[item];
    }

    std::vector <std::tuple <char, int>> set;
    set.reserve (_set.size ());
    for (const auto& pair : _set) {
        set.push_back (pair);
    }
    
    print_subsets (set, set.cbegin ());
}