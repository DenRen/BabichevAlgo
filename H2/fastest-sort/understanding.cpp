#include <iostream>
#include <map>

#include "solve.hpp"
#include "../../libs/other_func.hpp"

std::map <data_t, unsigned>
freq_map (const std::vector <data_t>& vec)
{
    std::map <data_t, unsigned> map;
    for (const auto& item : vec) {
        ++map[item];
    }

    return map;
}

int main () {
    data_t N = 400071;
    data_t K = 787;
    data_t M = 174;
    data_t L = 10000000;
    
    auto seq = calc_seq (N, K, M, L);
    int i = 543;
    auto pos = std::find (seq.cbegin () + i + 1, seq.cend (), seq[i]) - seq.cbegin ();
    std::cout << "seq[i]: " << seq[i] << ", pos: " << pos - i << std::endl;

    auto map = freq_map (seq);
    
    std::set <data_t> set_freq;
    data_t prev = 0;
    for (const auto&[value, freq] : map) {
        set_freq.insert (freq);
        // std::cout << value - prev << " " << value << ": " << freq << std::endl;
        prev = value;
    }
    std::cout << set_freq << std::endl;

    std::cout << "map size: " << map.size () << std::endl;
}