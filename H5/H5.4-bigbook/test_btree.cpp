#include <gtest/gtest.h>
#include "solve.hpp"

#include "../../libs/other_func.hpp"

TEST (BTREE_STATIC, INSERT_FIND) {
{
    btree_num_t tree;
    const int max_size = 10000;
    const key_t key_begin = -5000;

    key_t cur_key = key_begin;
    for (int size = 0; size <= max_size; ++size) {
        tree.insert (cur_key);

        for (int key = key_begin; key <= cur_key; ++key) {
            auto node_pos = tree.find (key);
            
            ASSERT_NE (node_pos.node, nullptr) << node_pos.node;
            ASSERT_EQ (node_pos.node->keys[node_pos.pos], key);
        }

        key_t key = cur_key + 1;
        for (int i = 0; i < size; ++i, ++key) {
            auto node_pos = tree.find (key);

            ASSERT_EQ (node_pos.node, nullptr) << key << " " << node_pos.node->is_leaf ();
        }

        ++cur_key;
    }
}
}

TEST (BTREE_RANDOM, INSERT_FIND_UNIQ) {
    btree_num_t tree;
    const int max_size = 10000;

    seclib::RandomGenerator rand;
    auto keys = rand.get_vector_uniq <key_t> (max_size);

    for (int size = 0; size < max_size; ++size) {
        tree.insert (keys[size]);

        for (int i = 0; i <= size; ++i) {
            auto node_pos = tree.find (keys[i]);

            ASSERT_NE (node_pos.node, nullptr) << node_pos.node;
            ASSERT_EQ (node_pos.node->keys[node_pos.pos], keys[i]);
        }

        for (int i = size + 1; i < max_size; ++i) {
            auto node_pos = tree.find (keys[i]);

            ASSERT_EQ (node_pos.node, nullptr) << node_pos.node;
        }
    }
}

TEST (BTREE_RANDOM, INSERT_FIND) {
    btree_num_t tree;
    const int max_size = 10000;

    seclib::RandomGenerator rand;
    auto keys = rand.get_vector <key_t> (max_size);
    std::set <key_t> inserted_keys;

    for (int size = 0; size < max_size; ++size) {
        key_t key = keys[size];
        tree.insert (key);
        inserted_keys.insert (key);

        for (int i = 0; i <= size; ++i) {
            auto node_pos = tree.find (key);

            ASSERT_NE (node_pos.node, nullptr) << node_pos.node;
            ASSERT_EQ (node_pos.node->keys[node_pos.pos], key);
        }

        for (int i = size + 1; i < max_size; ++i) {
            if (inserted_keys.find (keys[i]) == inserted_keys.end ()) {
                auto node_pos = tree.find (keys[i]);

                ASSERT_EQ (node_pos.node, nullptr) << node_pos.node;
            }
        }
    }
}

TEST (BTREE_RANDOM, REMOVE) {
{
    const int max_size = 10000;

    seclib::RandomGenerator rand;
    auto keys = rand.get_vector_uniq <key_t> (2 * max_size);

    btree_num_t tree;
    for (int i = 0; i < max_size; ++i) {
        tree.insert (keys[i]);
    }

    for (int i = 0; i < max_size; ++i) {
        ASSERT_TRUE (tree.remove (keys[i]));

        for (int j = 0; j < i; ++j) {
            ASSERT_FALSE (tree.remove (keys[j]));
        }

        for (int j = max_size; j < 2 * max_size; ++j) {
            ASSERT_FALSE (tree.remove (keys[j]));
        }
    }
}


}

