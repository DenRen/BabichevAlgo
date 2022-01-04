#include <vector>

#include "print_lib.hpp"

std::vector <unsigned> z_func (const std::string& str);

int main () {
    std::string str;
    std::cin >> str;

    std::cout << z_func (str) << std::endl;
}

std::vector <unsigned> z_func_naivly1 (const std::string& str) {
    std::vector <unsigned> res;
    res.reserve (str.size ());

    for (auto pos = str.cbegin () + 1; pos < str.cend (); ++pos) {
        auto cur_pos = pos, begin = str.cbegin (), end = str.cend ();
        while (cur_pos != end && *cur_pos == *begin) {
            ++cur_pos;
            ++begin;
        }

        res.emplace_back (cur_pos - pos);
    }

    return res;
}

std::vector <unsigned> z_func_naivly (const std::string& str) {
    std::vector <unsigned> res (str.size ());

    for (std::size_t j = 1; j < str.size (); ++j) {
        std::size_t p = j;
        while (p < str.size () && str[p] == str[p-j]) {
            ++p;
        }

        res[j] = p - j;
    }


    return res;
}

std::vector <unsigned> z_func_fast (const std::string& str) {
    std::vector <unsigned> res (str.size ());

    for (unsigned j = 1, l = 0, r = 0; j < str.size (); ++j) {
        auto p = j > r ? j : std::min (r + 1, j + res[j-l]);
        while (p < str.size () && str[p] == str[p-j]) {
            ++p;
        }

        res[j] = p - j;
        if (p > r) {
            l = j;
            r = p - 1;
        }

        // std::cout << "j: " << j << ", ";
        // std::cout << "p: " << p << ", ";
        // std::cout << "l: " << l << ", ";
        // std::cout << "r: " << r;
        // std::cout << std::endl << res << std::endl << std::endl;
    }

    return res;
}

std::vector <unsigned> z_func (const std::string& str) {
    return z_func_fast (str);
}