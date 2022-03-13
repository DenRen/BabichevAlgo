#define NDEBUG

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

int main () {
    unsigned N = 0;
    if (scanf ("%u", &N) != 1) {
        throw std::invalid_argument ("Read N failed");
    }
    solve (N, std::cin, std::cout);
}
