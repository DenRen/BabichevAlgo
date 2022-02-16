#include <iostream>
#include <array>
#include <algorithm>

struct freq_t {
    unsigned freq;
    char symbol;
};

void solve (const std::string& input_str) {
    // Init array
    const std::size_t alphabet_size = 'Z' - 'A' + 1;
    std::array <freq_t, alphabet_size> freq_table;
    for (std::size_t i = 0; i < alphabet_size; ++i) {
        freq_t freq = {
            .freq = 0,
            .symbol = static_cast <char> ('A' + i)
        };
        freq_table[i] = freq;
    }

    // Read input
    for (const auto& symb : input_str) {
        ++freq_table[symb - 'A'].freq;
    }

    // Special sort
    std::sort (freq_table.begin (), freq_table.end (),
        [] (const freq_t& lhs, const freq_t& rhs) {
            if (lhs.freq != rhs.freq) {
                return lhs.freq > rhs.freq;
            } else {
                return lhs.symbol < rhs.symbol;
            }
        }
    );

    // Output
    for (std::size_t i = 0; i < alphabet_size; ++i) {
        const freq_t& cur_elem = freq_table[i];
        if (cur_elem.freq != 0) {
            std::cout << cur_elem.symbol << " " << cur_elem.freq << std::endl;
        }
    }
}

int main () {
    std::string input_str;
    std::cin >> input_str;
    solve (input_str);
}