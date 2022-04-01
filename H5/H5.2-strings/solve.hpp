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
// #define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define DUMP(obj)
#endif

int
native_solve (const std::string& str) {
    int max_weight = 0;
    for (int size = 1; size <= str.size (); ++size) {
        std::map <std::string, int> substrs;
        for (int i = 0; i <= str.size () - size; ++i) {
            auto substr = str.substr (i, size);
            ++substrs[substr];
        }

        int max_num = 0;
        for (const auto& [substr, num] : substrs) {
            if (num > max_num) {
                max_num = num;
            }
        }

        int weight = max_num * size;
        if (weight > max_weight) {
            max_weight = weight;
        }
    }

    return max_weight;
}


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

std::size_t
find_num_doubled_substring (const std::string& str,
                            std::size_t size,
                            const std::vector <hash_t>& htab,
                            const std::vector <hash_t>& ptab)
{
    const auto num_substr = str.size () - size + 1;
    std::multimap <hash_t, std::pair <std::size_t, bool>> substr_hash;

    const auto last_ptab_index = str.size () - size;

    for (std::size_t i = 0; i < num_substr; ++i) {
        hash_t hash = htab[i + size] - htab[i];
        // Normilize:
        hash *= ptab[last_ptab_index - i];

        auto it = substr_hash.find (hash);
        const auto end = substr_hash.end ();
        if (it == substr_hash.end ()) {
            substr_hash.insert ({hash, {i, false}});
        } else {
            bool good = false;
            decltype (it) save_it;
            do {
                if (std::equal (str.begin () + i,
                                str.begin () + i + size,
                                str.begin () + it->second.first))
                {
                    it->second.second = true;
                    good = true;
                    break;
                }
                save_it = it++;
            } while (it != end && it->first == hash);
            
            if (!good) {
                substr_hash.insert (it, {hash, {i, false}});
            }
        }
    }

    int num_doubled = 0;
    for (const auto& [hash, pair] : substr_hash) {
        bool is_doubled = pair.second;
        num_doubled += is_doubled;
    }

    return num_doubled;
}

int
solve (const std::string& str) {
    auto ptab = calc_ptab (str.size (), 27);
    auto htab = calc_hash_table (str, ptab);

    long size = str.size () / 2;
    std::size_t size_step = size;
    long max_size = -1;
    std::size_t max_pos = 0;

    int max_weight = str.size ();
    std::size_t size_l = size - 1, size_r = str.size () - size_l - 1;
    while (true) {
        DUMP (size);

        int num_doubled = find_num_doubled_substring (str, size, htab, ptab);

        if (num_doubled != 0) {
            int weight = num_doubled * size;
            if (weight > max_weight) {
                max_weight = weight;
            }

            if (size_r == 0) {
                break;
            }

            // 3 / 2 == 1
            // 4 / 2 == 2

            auto dsize = size_r / 2 + 1; // > 0
            size += dsize;
            size_l = dsize - 1;
            size_r -= size_l + 1;
        } else {
            int weight = size;
            if (size_l == 0) {
                break;
            }

            auto dsize = size_l / 2 + 1;
            size -= dsize;
            size_r = dsize - 1;
            size_l -= size_r + 1;
        }
    }
    
    return max_weight;
}