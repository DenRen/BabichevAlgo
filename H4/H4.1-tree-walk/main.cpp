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

// g++ -DHOST -std=c++17 main.cpp

//#define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define dump(obj)
#endif

unsigned
solve (std::size_t max_str_len) {
    std::string str;
    str.reserve (max_str_len + 1);
    std::getline (std::cin, str);

    std::stack <unsigned> black_pos;
    
    // Parse input tree
    unsigned res = 0, deep = 0;
    for (char symb : str) {
        switch (symb) {
            case 'w': {
                res += !black_pos.empty ();
            } break;
            case 'b': {
                black_pos.push (deep);
            } break;
            case '(': {
                ++deep;
            } break;
            case ')': {
                if (!black_pos.empty () && deep == black_pos.top ()) {
                    black_pos.pop ();
                }
                --deep;
            } break;
            default:
                break;
        }
    }

    return res;
}

int main () {
    const std::size_t max_str_len = 10'000;
    std::cout << solve (max_str_len) << "\n";
}











































