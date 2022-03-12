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

#define NDEBUG

template <typename T>
struct node_t {
    T next[26] = {};
};

template <typename T>
std::ostream&
operator << (std::ostream& os, const node_t <T>& node) {
    os << node.next[0];
    for (int i = 1; i < 26; ++i) {
        os << " " << node.next[i];
    }
    return os;
}

template <typename T>
struct tree_t {
    std::vector <node_t <T>> buf;
    unsigned num_leafs = 0;

    tree_t () :
        buf (1)
    {}

    void
    add (const std::string& str) {
        T pos = 0;
        for (std::size_t i = 0; i < str.size (); ++i) {
            char symb = str[i];
            T& next_pos = buf[pos].next[symb - 'A'];
            if (next_pos != 0) {
                pos = next_pos;
                continue;
            }
            if (i != str.size () - 1) {
                ++num_leafs;
            } else {
                break;
            }

            next_pos = buf.size ();
            pos = buf.size ();
            buf.push_back ({}); // next_pos <- invalid
            ++i;

            for (; i < str.size (); ++i) {
                symb = str[i];
                buf[pos].next[symb - 'A'] = buf.size ();
                ++pos;
                buf.push_back ({});
            }
        }
    }
};

unsigned
solve (std::size_t number_worlds, std::size_t max_str_len) {
    std::string str;
    str.reserve (max_str_len);
    std::getline (std::cin, str);

    tree_t <unsigned short> tree;

    while (number_worlds--) {
        std::getline (std::cin, str);
        std::sort (str.begin (), str.end ());
        tree.add (str);
    }

    return tree.num_leafs;
}

int main () {
    const std::size_t max_str_len = 10'000;

    std::ios::sync_with_stdio (false);

    std::size_t N = 0;
    std::cin >> N;
    std::cout << solve (N, max_str_len) << std::endl;
}
