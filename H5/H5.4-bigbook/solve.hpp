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
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define dump(obj)
#endif

/*
    1. Сделать реализацию B-tree с числами и протестировать её
    2. Заменить ключи на hash_t и снова всё протестировать
    3. Заменить value на std::string и посмотреть, что всё не упало
    4. Придёмать механизм размещения узлов и KV пар в SSD
*/

const auto NUM_KEYS = 4;
typedef uint32_t hash_t;

class btree_num_t {
public:
    struct node_t {
        unsigned size = 0;
        std::array <int,     NUM_KEYS> keys {-1};           // For kill
        std::array <node_t*, NUM_KEYS + 1> poss {nullptr};  // For work

        unsigned
        get_M () const noexcept {
            return size + 1;
        }
    };

    node_t root;

    void
    insert (int key) {
        if (root.size == 0) {
            root.keys[0] = key;
            return;
        }

        node_t* cur_node = &root;
        for (int i = 0; i < cur_node->size; ++i) {
            const auto& cur_key = cur_node->keys[i];
            if (key < cur_key) {
                auto* next_node = cur_node->poss[i];
                if (next_node != nullptr) {
                    i = 0;
                    cur_node = next_node;
                    continue;
                }


            }
        }
    }

    static bool
    is_leaf (node_t* node) noexcept {
        return node->poss[0] == nullptr;
    }

    static unsigned
    num_pos (node_t* node) noexcept {
        return node->size + 1;
    }

    void
    destruct_childs (node_t* node) {
        if (is_leaf (node)) {
            return;
        }

        for (int i = 0; i < num_pos (node); ++i) {
            destruct_childs (node->poss[i]);
            delete node->poss[i];
            node->poss[i] = nullptr;
        }
    }

    ~btree_num_t () {
        if (root.size == 0) {
            return;
        }

        destruct_childs (&root);
    }

private:
    std::ostream&
    draw_node_keys (std::ostream& os, const node_t& node, unsigned index) const {
        if (node.size == 0) {
            return os;
        }

        os << "node" << index << "[label = \"<p0>";
        for (int i = 0; i < node.size; ++i) {
            os << "| <k" << i << "> " << node.keys[i]
            << "| <p" << i+1 << ">";
        }
        return os << "\"];\n";
    }

    void
    draw_arrow (std::ostream& os,
                unsigned index_from, unsigned index_to,
                unsigned index_ptr, unsigned index_key) const {
        // "node0":f2 -> "node4":f1;
        os << "\"node" << index_from << "\":p" << index_ptr << " -> "
           << "\"node" << index_to   << "\":k" << index_key << ";\n";
    }

    unsigned
    draw_node (std::ostream& os, const node_t* node, unsigned index = 0) const {
        if (node == nullptr) {
            return index;
        }

        const auto parent_index = index;
        draw_node_keys (os, *node, parent_index);
        for (int i = 0; i < node->size + 1; ++i) {
            if (node->poss[i] != nullptr) {
                auto new_index = draw_node (os, node->poss[i], index + 1);
                draw_arrow (os, parent_index, index + 1, i, (node->poss[i]->size - 1) / 2);
                index = new_index;
            }
        }

        return index;
    }

public:
    void
    draw () const {
        std::fstream os {"graph.gv", std::ios_base::out};
        if (root.size == 0) {
            return;
        }

        os << "digraph G {\n"
              "node [shape = record, height=.1];\n";
        draw_node (os, &root);
        os << "}";
    }
};

void
solve (std::size_t num_req, std::ostream& os, std::size_t max_ram) {

}