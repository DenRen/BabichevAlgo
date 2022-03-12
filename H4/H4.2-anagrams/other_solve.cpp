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

// #define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define dump(obj)
#endif

template <typename T>
struct templ_t {
    T freq[26];

    void
    add (char symb) {
        ++freq[symb - 'A'];
    }
};

template <typename T>
int
operator < (const templ_t <T>& lhs, const templ_t <T>& rhs) {
    std::size_t i = 0;
    while (lhs.freq[i] == rhs.freq[i]) {
        if (++i == 26) {
            return 0;
        }
    }

    return lhs.freq[i] < rhs.freq[i];
}

template <typename T>
void
str2templ (const std::string& str, templ_t <T>& templ) {
    for (char symb : str) {
        templ.add (symb);
    }
}

template <typename T>
unsigned
solve (std::size_t number_worlds, std::size_t max_str_len) {
    std::string str;
    str.reserve (max_str_len);
    std::getline (std::cin, str);

    std::set <templ_t <T>> complects;

    while (number_worlds--) {
        std::getline (std::cin, str);
        
        templ_t <T> templ = {};
        str2templ (str, templ);
        complects.insert (templ);
    }

    return complects.size ();
}

int main () {
    const std::size_t max_str_len = 10'000;

    std::ios::sync_with_stdio (false);

    std::size_t N = 0;
    std::cin >> N;
    std::cout << solve <unsigned short> (N, max_str_len) << std::endl;
}