#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <map>

#include "other_func.hpp"

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

unsigned inline
hash_symb (char symb) {
    return symb - 'a' + 1;
}

unsigned
hash (const std::string& str,
      unsigned begin,
      unsigned end,
      const std::vector <unsigned>& ptab)
{
    unsigned sum = 0;
    for (unsigned i = begin; i < end; ++i) {
        sum += (str[i] - 'a' + 1) * ptab[i - begin];
    }

    return sum;
}


int karp_rabin (std::string const& s1,
                std::string const& s2,
                std::vector <unsigned> const& ptab)
{
    unsigned hs1 = hash (s1, 0, s1.size (), ptab);

    std::vector <unsigned> htab (s2.size ());
    for (unsigned i = 1; i < s2.size (); i++)
        htab[i] = htab[i-1] + (s2[i-1] - 'a' + 1) * ptab[i-1];
    
    for (unsigned i = 0; i < s2.size() - s1.size(); i++) {
        unsigned hs2 = htab[i+s1.size()] - htab[i];
        std::cout << "hs2: " << hs2 << std::endl;
        std::cout << "hs1: " << hs1 << std::endl;
        if (hs2 == hs1) {
            bool ok = true;
            for (unsigned j = 0; j < s1.size(); j++)
                if (s1[j] != s2[i+j])
                    ok = false;
        
            if (ok)
                return i;
        }
        
        hs1 *= ptab[1];
    }

    return -1;
}
