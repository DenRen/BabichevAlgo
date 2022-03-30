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

// g++ -DHOST -std=c++17 main.cpp

// #define NDEBUG
#define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define DUMP(obj)
#endif

// Karb rabin

typedef uint32_t hash_t;

hash_t
symb2num (char c) {
    return c - 'a' + 1;
}

std::vector <hash_t>
calc_ptab (unsigned n_max, hash_t p) {
    std::vector <hash_t> ptab (n_max + 1);
    ptab[0] = 1;
    for (std::size_t i = 1; i < n_max; ++i) {
        ptab[i] = p * ptab[i - 1];
    }

    return ptab;
}

// always: res[0] == 0
std::vector <hash_t>
calc_hash_table (const std::string& str,
                 const std::vector <hash_t>& ptab)
{
    std::vector <hash_t> htab (str.size () + 1);
    htab[0] = 0;
    for (std::size_t i = 0; i < str.size (); ++i) {
        htab[i + 1] = htab[i] + ptab[i] * symb2num (str[i]);
    }

    return htab;
}

hash_t
calc_hash (const std::string& str,
           const std::vector <hash_t>& ptab)
{
    hash_t hash = symb2num (str[0]);
    for (std::size_t i = 1; i < str.size (); ++i) {
        hash += symb2num (str[i]) * ptab[i];
    }

    return hash;
}

std::size_t
find_substr_kr (const std::string& src,
                const std::string& pat,
                const std::vector <hash_t>& ptab)
{
    auto htab = calc_hash_table (src, ptab);
    hash_t pat_hash = calc_hash (pat, ptab);

    const hash_t p = ptab[1];
    for (std::size_t i = 0; i <= src.size () - pat.size (); ++i) {
        unsigned l = i, r = l + pat.size ();
        hash_t substr_hash = htab[r] - htab[l];

        if (substr_hash == pat_hash) {
            if (std::equal (pat.cbegin (), pat.cend (), src.cbegin () + l)) {
                return l;
            }
        }

        pat_hash *= p;
    }

    return src.end () - src.begin ();
}

std::string
solve (const std::string& str) {

}