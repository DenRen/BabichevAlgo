#include "../../libs/other_func.hpp"
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
native_solve (std::size_t number_worlds, std::size_t max_str_len) {
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

template <typename T>
void
gen_test (std::string in_file_name, std::string out_file_name) {
    std::fstream in  {in_file_name,  std::ios_base::out};
    std::fstream out {out_file_name, std::ios_base::out};

    const int N = 100000;
    const int L = 10000;

    in << N << "\n";
    seclib::RandomGenerator rand;
    std::set <templ_t <T>> complects;

    for (int i = 0; i < N; ++i) {
        std::string str = rand.get_string (L, 'A', 'D');
        in << str << "\n";

        templ_t <T> templ = {};
        str2templ (str, templ);
        complects.insert (templ);
    }

    out << complects.size () << "\n";
}

int main () {
    gen_test <unsigned short> ("test", "test_ref");
}