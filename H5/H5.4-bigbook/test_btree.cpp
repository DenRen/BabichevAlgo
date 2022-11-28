#include <gtest/gtest.h>
#include "btree.hpp"

#include "../../libs/other_func.hpp"

using Key = int;
using Tree = BTree<Key>;

TEST (BTREE_STATIC, INSERT_FIND)
{
    auto test = [](unsigned btree_num_keys,
                   int max_size,
                   Key key_begin)
    {
        Tree tree{ btree_num_keys };

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
    };

    test(1, 100, -51);
    test(2, 200, -50);
    test(13, 1300, -50);
    test(2048, 2000, -500);
    test(4096, 10'000, -5000);
}

TEST (BTREE_RANDOM, INSERT_FIND_UNIQ)
{
    auto test = [](unsigned btree_num_keys,
                   int max_size)
    {
        Tree tree{ btree_num_keys };
        seclib::RandomGenerator rand;
        auto keys = rand.get_vector_uniq <key_t> (2 * max_size);

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
    };

    test(1, 100);
    test(2, 100);
    test(7, 100);
    test(7, 100);
    test(13, 200);
    test(512, 5000);
    test(1024, 5000);
    test(4096, 5000);
}

TEST (BTREE_RANDOM, INSERT_FIND)
{
    auto test = [](unsigned btree_num_keys,
                   int max_size)
    {
        Tree tree{ btree_num_keys };

        seclib::RandomGenerator rand;
        auto keys = rand.get_vector <key_t> (max_size);
        std::set <key_t> inserted_keys;

        for (int size = 0; size < max_size; ++size) {
            key_t key = keys[size];
            tree.insert (key);
            inserted_keys.insert (key);

            for (int i = 0; i <= size; ++i) {
                key_t key = keys[i];
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
    };

    test(1, 100);
    test(2, 100);
    test(7, 100);
    test(7, 100);
    test(13, 200);
    test(512, 5000);
    test(1024, 5000);
    test(4096, 5000);
}

TEST (BTREE_RANDOM, REMOVE)
{

    auto test = [](unsigned btree_num_keys,
                   int max_size)
    {
        seclib::RandomGenerator rand;
        auto keys = rand.get_vector_uniq <key_t> (2 * max_size);

        Tree tree;
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
    };

    test(1, 100);
    test(2, 100);
    test(7, 100);
    test(7, 100);
    test(13, 200);
    test(512, 5000);
    test(1024, 5000);
    test(4096, 5000);

}

