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

    tree_t () :
        buf (1)
    {}

    void
    add (const std::string& str) {
        T pos = 0;
        for (char symb : str) {
            T& next_pos = buf[pos].next[symb - 'A'];
            if (next_pos == 0) {
                next_pos = buf.size ();
                buf.push_back ({});
                pos = buf[pos].next[symb - 'A'];
            } else {
                pos = next_pos;
            }
        }
    }

    std::size_t
    num_leafs_impl (T begin_pos) const {
        std::stack <std::pair <T, int>> parent;
        parent.push ({begin_pos, 0});

        while (!parent.empty ()) {
            auto[pos, i] = parent.top ();
            for (i = 0; i < 26; ++i) {
                if (buf[pos].next[i] != 0) {
                    parent.push ({pos, i});

                }                
            }
        }

        const node_t <T>& node = buf[pos];


        std::size_t res = 0;
        for (int i = 0; i < 26; ++i) {
            if (node.next[i] != 0) {
                res += num_leafs_impl (node.next[i]);
            }
        }
        
        return res ? res : 1;
    }

    unsigned
    num_leafs () const {
        T root_pos = 0;
        return num_leafs_impl (root_pos);
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

    return tree.num_leafs ();
}

int main () {
    const std::size_t max_str_len = 10'000;

    std::size_t N = 0;
    std::cin >> N;
    std::cout << solve (N, max_str_len) << std::endl;
}
