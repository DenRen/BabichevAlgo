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

/*
    1. Сделать реализацию B-tree с числами и протестировать её
    2. Заменить ключи на hash_t и снова всё протестировать
    3. Заменить value на std::string и посмотреть, что всё не упало
    4. Придёмать механизм размещения узлов и KV пар в SSD
*/

template <typename T, std::size_t N>
std::ostream&
print_array (std::ostream& os,
             const std::array <T, N>& arr,
             std::size_t size)
{
    if constexpr (N != 0) {
        if (size == 0) {
            return os;
        }

        os << arr[0];
        for (std::size_t i = 1; i < std::min (N, size); ++i) {
            os << ", " << arr[i];
        }
    }

    return os;
}

const auto MAX_NUM_KEYS = 3;
typedef uint32_t hash_t;

class btree_num_t {
public:
    struct node_t {
        unsigned size = 0;
        std::array <int,     MAX_NUM_KEYS> keys {-1};           // For kill
        std::array <node_t*, MAX_NUM_KEYS + 1> poss {nullptr};  // For work

        unsigned
        num_pos () const noexcept {
            return size + 1;
        }

        unsigned
        num_keys () const noexcept {
            return size;
        }

        bool
        is_leaf () const noexcept {
            return poss[0] == nullptr;
        }

        std::ostream&
        dump (std::ostream& os = std::cout) const {
            os << "{";
            if (num_keys () != 0) {
                os << "keys = [";
                print_array (os, keys, num_keys ())
                    << "], poss = [";
                print_array (os, poss, num_pos ())
                    << "]";
            }
            return os << "}";
        }

        std::array <int, MAX_NUM_KEYS>::iterator
        keys_begin () noexcept {
            return keys.begin ();
        }
        
        std::array <int, MAX_NUM_KEYS>::iterator
        keys_end () noexcept {
            return keys_begin () + num_keys ();
        }

        std::array <node_t*, MAX_NUM_KEYS>::iterator
        poss_begin () noexcept {
            return poss.begin ();
        }

        std::array <node_t*, MAX_NUM_KEYS + 1>::iterator
        poss_end () noexcept {
            return poss_begin () + num_pos ();
        }

        void
        insert (int key, node_t* pos) {
            if (num_keys () == MAX_NUM_KEYS) {
                throw std::runtime_error ("insert in already full node");
            }

            const auto it_key = std::upper_bound (keys.begin (), keys_end (), key);
            std::copy (it_key, keys_end (), it_key + 1);
            *it_key = key;

            const auto it_pos = poss.begin () + (it_key - keys.begin ());
            std::copy (it_pos, poss_end (), it_pos + 1);
            *it_pos = pos;

            ++size;
        }
    };

    node_t* root = new node_t;

private:
    void
    insert_parts (node_t* node,
                  int key, node_t* pos,
                  std::stack <std::pair <node_t*, int>>& stack)
    {
        const auto num_keys = node->num_keys ();
        if (num_keys < MAX_NUM_KEYS) {
            node->insert (key, pos);
            return;
        } else if (num_keys > MAX_NUM_KEYS) {
            node->dump (std::cerr);
            throw std::runtime_error ("over num keys");
        }

        // Here node->num_keys () == MAX_NUM_KEYS
        auto middle_i = num_keys / 2;
        int middle_key = node->keys[middle_i];

        // Create left node
        node_t* left_node = new node_t;
        std::copy (node->keys_begin (), node->keys_begin () + middle_i,
                   left_node->keys_begin ());
        std::copy (node->poss_begin (), node->poss_begin () + middle_i + 1,
                   left_node->poss_begin ());
        left_node->size = middle_i;
        node_t* middle_pos = left_node;

        // Create right node // todo err
        auto begin_key = middle_i + 1;
        auto begin_pos = middle_i + 2-1;
        std::copy (node->keys_begin () + begin_key, node->keys_end (),
                   node->keys_begin ());
        std::copy (node->poss_begin () + begin_pos, node->poss_end (),
                   node->poss_begin ());
        node->size -= 1 + middle_i;
        
        // Insert key and pos
        if (key < middle_key) {
            left_node->insert (key, pos);
        } else {
            node->insert (key, pos);
        }

        if (stack.size () != 0) {
            node_t* cur_parent = stack.top ().first;
            stack.pop ();
            insert_parts (cur_parent, middle_key, middle_pos, stack);
        } else {
            node_t* right_node = node;
            assert (right_node == root);

            root = new node_t;
            root->size = 1;
            root->keys[0] = middle_key;
            root->poss[0] = left_node;
            root->poss[1] = right_node;
        }
    }

    void
    insert_impl (node_t* node,
                 int key,
                 std::stack <std::pair <node_t*, int>>& stack)
    {
        /*node->dump () << std::endl;

        if (node->num_keys () == MAX_NUM_KEYS) {
            auto middle_pos = node->num_keys () / 2;
            int middle_key = node->keys[middle_pos];

            // Create right node
            node_t* new_node = new node_t;
            auto begin_key = middle_pos + 1;
            auto begin_pos = middle_pos + 2;

            // Copy right data to right node
            new_node->size = node->num_keys () - middle_pos - 1;
            for (int i = 0; i < new_node->num_keys (); ++i) {
                new_node->keys[i] = node->keys[begin_key + i];
                new_node->poss[i] = node->poss[begin_pos + i];
                node->keys[begin_key + i] = -1;
                node->poss[begin_pos + i] = nullptr;
            }
            new_node->poss[new_node->num_pos () - 1] = node->poss[node->num_pos () - 1];

            node->size = middle_pos;
            if (key < middle_key) {
                node->insert_in_leaf (key);
            } else {
                new_node->insert_in_leaf (key);
            }
        } else {
            if (node->is_leaf ()) {
                node->insert_in_leaf (key);
            }
        }*/
    }

    std::pair <std::stack <std::pair <node_t*, int>>,
               node_t*>
    get_path (int key) {
        std::stack <std::pair <node_t*, int>> stack;

        node_t* cur_node = root;
        for (int i = 0; i < cur_node->num_keys (); ++i) {
            if (key < cur_node->keys[i]) {
                if (cur_node->is_leaf ()) {
                    return {stack, cur_node};
                }

                stack.push ({cur_node, i});
                cur_node = cur_node->poss[i];
                i = -1;
            } else if (i + 1 == cur_node->num_keys ()) {
                if (cur_node->is_leaf ()) {
                    return {stack, cur_node};
                }

                stack.push ({cur_node, i});
                cur_node = cur_node->poss[i + 1];
                i = -1;
            }
        }

        assert (cur_node == root);
        return {{}, root};
    }

public:
    void
    insert (int key) {
        auto[parent_stack, node] = get_path (key);
        node->dump () << std::endl;
        insert_parts (node, key, nullptr, parent_stack);
    }

    static unsigned
    num_pos (node_t* node) noexcept {
        return node->size + 1;
    }

    void
    destruct_childs (node_t* node) {
        if (node->is_leaf ()) {
            return;
        }

        for (int i = 0; i < node->num_pos (); ++i) {
            destruct_childs (node->poss[i]);
            delete node->poss[i];
            node->poss[i] = nullptr;
        }
    }

    ~btree_num_t () {
        if (root->size == 0) {
            return;
        }

        // destruct_childs (root);
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
                ++index;
                node_t* child = node->poss[i];
                auto new_index = draw_node (os, child, index);
                draw_arrow (os, parent_index, index, i, (child->size - 1) / 2);
                index = new_index;
            }
        }

        return index;
    }

public:
    void
    draw () const {
        static unsigned number_dump = 0;

        std::string name = "graph_";
        (name += std::to_string (number_dump)) += ".gv";
        std::fstream os {name, std::ios_base::out};
        if (root->size == 0) {
            return;
        }

        os << "digraph G {\n"
              "node [shape = record, height=.1];\n";
        draw_node (os, root);
        os << "}";

        ++number_dump;
    }
};

void
solve (std::size_t num_req, std::ostream& os, std::size_t max_ram) {

}