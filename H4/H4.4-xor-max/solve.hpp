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

#define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define dump(obj)
#endif

class xor_array_native {
    std::set <unsigned> arr;
public:
    void
    insert (unsigned value) {
        arr.insert (value);
    }

    void
    remove (unsigned value) {
        arr.erase (value);
    }

    unsigned
    max_xor (unsigned value) const {
        std::pair <unsigned, unsigned> max_elem;
        for (const auto& elem : arr) {
            max_elem = std::max (max_elem, {elem ^ value, elem},
                [] (const auto& lhs, const auto& rhs) {
                    return lhs.first < rhs.first;
                });
        }
        return max_elem.second;
    }
};
/*
class xor_array {

public:
    void
    insert (unsigned value) {

    }

    void
    remove (unsigned value) {

    }

    unsigned
    max_xor (unsigned value) const {
        return -1;
    }
};
*/

class xor_array_map {
    std::vector <unsigned> num_buf;
    //        value,    pos
    std::map <unsigned, unsigned> map;
public:
    void
    insert (unsigned value) {
        if (map.find (value) == map.cend ()) {
            map[value] = num_buf.size ();
            num_buf.push_back (value);
        }
    }

    void
    remove (unsigned rm_value) {
        auto rm_it = map.find (rm_value);
        if (rm_it == map.end ()) {
            return;
        }

        if (map.size () == 1) {
            num_buf.clear ();
            map.clear ();
            return;
        }

        auto rm_pos = rm_it->second;
        map.erase (rm_it);
        if (const auto last_pos = num_buf.size () - 1; rm_pos != last_pos) {
            auto last_value = num_buf[last_pos];
            num_buf[rm_pos] = last_value;
            map[last_value] = rm_pos;
        }
        num_buf.pop_back ();
    }

    unsigned
    max_xor (unsigned value) const {
        // Don't use here std::max, it decrees perf!!!
        unsigned max_value = 0, max_elem = 0;
        for (const auto& elem : num_buf) {
            auto cur_val = elem ^ value;
            if (cur_val > max_value) {
                max_value = cur_val;
                max_elem = elem;
            }
        }

        return max_elem;
    }
};

class xor_array_bt {
    struct node_t {
        node_t* left  = nullptr;
        node_t* right = nullptr;
    };

    node_t root;

public:
    void
    insert (unsigned value) {
        node_t* node = &root;
        unsigned mask = 1u << 31;
        while (mask) {
            bool bit = (value & mask) != 0;
            mask >>= 1;

            node_t** next_node = bit ? &node->right : &node->left;
            if (*next_node == nullptr) {
                *next_node = new node_t;
            }
            node = *next_node;
        }
    }

    void
    remove (unsigned value) {
        node_t* parents[32] = {nullptr};

        node_t* node = &root;
        unsigned mask = 1u << 31;
        int i = 0;
        for (; i < 32; ++i) {
            parents[i] = node;

            unsigned bit = (value & mask) != 0;

            node_t* next_node = bit ? node->right : node->left;
            if (next_node == nullptr) {
                return;
            }
            node = next_node;

            mask >>= 1;
        }

        --i;
        mask = 1u;
        for (; i >= 0; --i) {
            unsigned bit = (value & mask) != 0;

            node_t* parent = parents[i];
            node_t** parent_to_me = bit ? &parent->right : &parent->left;
            delete *parent_to_me;
            *parent_to_me = nullptr;

            node_t* parent_to_other = !bit ? parent->right : parent->left;
            if (parent_to_other != nullptr) {
                return;
            }

            mask <<= 1;
        }
    }

    unsigned
    max_xor (unsigned value) const {
        value = ~value;

        unsigned res = 0;
        const node_t* node = &root;
        unsigned mask = 1u << 31;
        while (mask) {
            unsigned bit = (value & mask) != 0;
            mask >>= 1;

            res <<= 1;
            node_t* next_node = bit ? node->right : node->left;
            if (next_node == nullptr) {
                res |= !bit;
                node = !bit ? node->right : node->left;
            } else {
                res |= bit;
                node = next_node;
            }
        }

        return res;
    }
};

void
solve (unsigned N, std::istream& is, std::ostream& os) {
    unsigned type = 0, value = 0;

    xor_array_bt arr;

    while (N--) {
        if (scanf ("%u %u", &type, &value) != 2) {
            throw std::invalid_argument ("Read type or value failed");
        }
        switch (type) {
            case 1: {
                arr.insert (value);
            } break;
            case 2: {
                arr.remove (value);
            } break;
            case 3: {
                printf ("%u\n", arr.max_xor (value));
            } break;
            default:
                throw std::invalid_argument ("Error input");
        }
    }
}