#include <gtest/gtest.h>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

template <typename DB>
void
test_db_static_insert_find () {
    DB tree;
    const int max_size = 1000;
    const int key_begin = -500;

    int cur_key = key_begin;
    for (int size = 0; size <= max_size; ++size) {
        tree.insert (std::to_string (cur_key), "lol");

        for (int key = key_begin; key <= cur_key; ++key) {
            auto it = tree.find (std::to_string (key));

            ASSERT_NE (it, tree.end ());
            ASSERT_EQ (it->first, std::to_string (key));
        }

        int key = cur_key + 1;
        for (int i = 0; i < size; ++i, ++key) {
            auto it = tree.find (std::to_string (key));

            ASSERT_EQ (it, tree.end ());
        }

        ++cur_key;
    }
}

template <typename DB>
void
test_db_static_insert_find_uniq () {
    DB tree;
    const int max_size = 1000;
    const int key_begin = -500;

    seclib::RandomGenerator rand;
    auto keys = rand.get_vector_uniq <key_t> (2 * max_size);

    for (int size = 0; size < max_size; ++size) {
        tree.insert (std::to_string (keys[size]), "lol");

        for (int i = 0; i <= size; ++i) {
            auto it = tree.find (std::to_string (keys[i]));

            ASSERT_NE (it, tree.end ());
            ASSERT_EQ (it->first, std::to_string (keys[i]));
        }

        for (int i = size + 1; i < size; ++i) {
            auto it = tree.find (std::to_string (keys[i]));

            ASSERT_EQ (it, tree.end ());
        }
    }
}

template <typename DB>
void
test_db_random_insert_find () {
    const int max_size = 1000;

    seclib::RandomGenerator rand;
    auto keys = rand.get_vector <int> (max_size);
    std::set <int> inserted_keys;

    DB tree;
    for (int size = 0; size < max_size; ++size) {
        auto key = std::to_string (keys[size]);
        tree.insert (key, "lol");
        inserted_keys.insert (keys[size]);

        for (int i = 0; i <= size; ++i) {
            auto key = std::to_string (keys[i]);
            auto it = tree.find (key);

            ASSERT_NE (it, tree.end ());
            ASSERT_EQ (it->first, key);
        }

        for (int i = size + 1; i < max_size; ++i) {
            if (inserted_keys.find (keys[i]) == inserted_keys.end ()) {
                auto it = tree.find (std::to_string (keys[i]));

                ASSERT_EQ (it, tree.end ());
            }
        }
    }
}

template <typename DB>
void
test_db_random_remove () {
    const int max_size = 1000;

    seclib::RandomGenerator rand;
    auto keys = rand.get_vector_uniq <int> (2 * max_size);

    DB tree;
    for (int i = 0; i < max_size; ++i) {
        tree.insert (std::to_string (keys[i]), "lol");
    }

    for (int i = 0; i < max_size; ++i) {
        ASSERT_TRUE (tree.remove (std::to_string (keys[i])));

        for (int j = 0; j < i; ++j) {
            ASSERT_FALSE (tree.remove (std::to_string (keys[j])));
        }

        for (int j = max_size; j < 2 * max_size; ++j) {
            ASSERT_FALSE (tree.remove (std::to_string (keys[j])));
        }
    }
}

TEST (NATIVE_DB_STATIC,  INSERT_FIND)      { test_db_static_insert_find      <db_native::data_base_t> (); }
TEST (NATIVE_DB_RANDOM,  INSERT_FIND_UNIQ) { test_db_static_insert_find_uniq <db_native::data_base_t> (); }
TEST (NATIVE_DB_RANDOM,  INSERT_FIND)      { test_db_static_insert_find_uniq <db_native::data_base_t> (); }
TEST (NATIVE_DB_RANDOM,  REMOVE)           { test_db_random_remove           <db_native::data_base_t> (); }

TEST (RELEASE_DB_STATIC, INSERT_FIND)      { test_db_static_insert_find      <db::data_base_t> (); }
TEST (RELEASE_DB_RANDOM, INSERT_FIND_UNIQ) { test_db_static_insert_find_uniq <db::data_base_t> (); }
TEST (RELEASE_DB_RANDOM, INSERT_FIND)      { test_db_static_insert_find_uniq <db::data_base_t> (); }
TEST (RELEASE_DB_RANDOM, REMOVE)           { test_db_random_remove           <db::data_base_t> (); }