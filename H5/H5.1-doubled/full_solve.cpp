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

#define NDEBUG
// #define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define DUMP(obj)
#endif

struct node_t {
private:
    const unsigned begin_size = 26;

public:
    unsigned size;
    std::vector <char> symb;
    std::vector <node_t*> pos;

    node_t (unsigned size = 0) :
        size (0),
        symb (begin_size),
        pos (begin_size)
    {}

    decltype (symb)::iterator
    symb_end () {
        return symb.begin () + size;
    }

    // Был до этого и указатель на следующий элемент
    std::pair <bool, node_t*>
    add (char c) {
        auto it = std::lower_bound (symb.begin (), symb_end (), c);
        const auto index = it - symb.begin ();
        if (it != symb_end () && *it == c) {
            return {true, pos[index]};
        }

        node_t* new_node = new node_t;
        symb.insert (it, c);
        pos.insert (pos.begin () + index, new_node);
        ++size;

        return {false, new_node};
    }
};

struct tree_t {
    node_t* root = new node_t;

    // Возвращает размер совпадения
    std::size_t
    insert (const std::string& str,
            std::size_t begin,
            std::size_t end)
    {
        std::size_t len_collision = 0;
        node_t* cur_node = root;
        for (auto i = begin; i < end; ++i) {
            auto[is_was, next] = cur_node->add (str[i]);
            cur_node = next;

            len_collision += is_was;
        }

        return len_collision;
    }
};

std::string
solve (const std::string& str) {
    tree_t tree;

    std::size_t max_match_size = 0, i_max_match = 0;
    std::size_t i = 0;
    for (; i < str.size (); ++i) {
        std::size_t len_collision = tree.insert (str, i, str.size ());
        if (len_collision > max_match_size) {
            max_match_size = len_collision;
            i_max_match = i;
        }
    }

    const std::size_t len = str.size () - i_max_match - 1;
    return str.substr (i_max_match, max_match_size);
}

int main () {
    std::string str;
    std::getline (std::cin, str);
    std::cout << solve (str) << "\n";
}