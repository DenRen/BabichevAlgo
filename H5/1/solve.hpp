#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <map>

#include "other_func.hpp"

/* Optimize:
    1) stirng.at (i) -> string.data ()[i]
*/

template <typename HashT>
std::vector <HashT>
gen_ptab (std::size_t size,
          const HashT p)
{
    std::vector <HashT> ptab (size);
    HashT mul = 1;
    
    for (auto& item : ptab) {
        item = mul;
        mul *= p;
    }

    return ptab;
}

template <typename HashT = uint32_t>
HashT hash (const std::string& str,
            std::size_t begin,
            std::size_t end,
            const std::vector <HashT>& ptab)
{
    HashT hash = 0;
    for (std::size_t i = begin; i < end; ++i) {
        hash += str[i] * ptab[i - begin];
    }

    return hash;
}

template <typename HashT>
std::vector <HashT>
calc_hash_table (const std::string str,
                 const std::vector <HashT>& ptab)
{
    const auto size = str.size ();

    std::vector <HashT> htab (size + 1);
    for (std::size_t i = 0; i < size; ++i) {
        htab[i + 1] = htab[i] + str[i] * ptab[i];
    }

    return htab;
}

template <typename HashT = uint32_t>
int
karp_rabin_impl (HashT pattern_hash,
                 std::string::const_iterator pattern,
                 std::size_t pattern_size,
                 std::string::const_iterator source,
                 std::size_t source_size,
                 const std::vector <HashT>& ptab,
                 const std::vector <HashT>& hash_table)
{
   const auto p = ptab[1];

    for (std::size_t i = 0; i <= source_size - pattern_size; ++i) {
        auto cur_hash = hash_table[i + pattern_size] - hash_table[i];
        if (cur_hash == pattern_hash) {
            bool ok = true;
            for (std::size_t j = 0; j < pattern_size; ++j) {
                if (pattern[j] != source[j + i]) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                return i;
            }
        }

        pattern_hash *= p;
    }

    return -1;
}

template <typename HashT = uint32_t>
int karp_rabin (const std::string& pattern,
                const std::string& source,
                const std::vector <HashT>& ptab)
{
    const auto pattern_size = pattern.size ();
    const auto source_size = source.size ();

    if (source_size < pattern_size) {
        return -1;
    } 

    auto pattern_hash = hash (pattern, 0, pattern_size, ptab);
    std::vector <HashT> hash_table = calc_hash_table (source, ptab);

    const auto p = ptab[1];

    for (std::size_t i = 0; i <= source_size - pattern_size; ++i) {
        auto cur_hash = hash_table[i + pattern_size] - hash_table[i];
        if (cur_hash == pattern_hash) {
            bool ok = true;
            for (std::size_t j = 0; j < pattern_size; ++j) {
                if (pattern[j] != source[j + i]) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                return i;
            }
        }

        pattern_hash *= p;
    }

    return -1;
}

template <typename T, typename U>
T pow_integer (T number, U power) {
    T res = 1;
    while (power--) {
        res *= number;
    }

    return res;
}

template <typename HashT = uint32_t>
std::string solve (std::string src) {
    const auto src_size = src.size ();

    const HashT size_ptab = 260;
    const HashT p = 263;

    auto ptab = gen_ptab <HashT> (size_ptab, p);
    auto htab = calc_hash_table (src, ptab);
    
    std::string res;
    bool previous_is_find = false;

    // for (std::size_t size = src_size / 2; size != 0; --size) {
    for (std::size_t size = 1; size < src_size; ++size) {
        //std::cout << "size: " << size << std::endl;

        std::size_t pos = 0;
        for (; pos <= src_size - size; ++pos) {
            HashT pattern_hash = htab[pos + size] - htab[pos];
            pattern_hash *= p;

            // std::cout << "\tpattern: " << src.substr (pos, size) 
            //           << ", hash: " << pattern_hash << std::endl;
    
            for (std::size_t i = pos + 1; i <= src_size - size; ++i) {
                auto cur_hash = htab[i + size] - htab[i];
                // std::cout << "\tcur: " << src.substr (i, size)
                //           << ", hash: " << cur_hash
                //           << ", pattern hash: " << pattern_hash << std::endl;

                if (cur_hash == pattern_hash) {
                    // std::cout << " true!" << std::endl;
                    if (src.compare (pos, size, src, i, size) == 0) {
                        previous_is_find = true;
                        res = src.substr (pos, size);

                        // std::cout << "i: " << i << ", find: " << res << std::endl;

                        pos = INT64_MAX - 1;
                        break;
                    }
                }

                pattern_hash *= p;
            }
        }
        
        if (pos != INT64_MAX && previous_is_find == true) {
            return res;
        }
    }

    return res;
}
