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

TEST (RELEASE_DB_USE_NATIVE, FULL) {
    const int key_len_min = 1, key_len_max = 4096;
    const char symb_begin = 'a', symb_end = 'z';

    seclib::RandomGenerator rand;

    auto gen_str = [&] () {
        const auto len = rand.get_rand_val <unsigned> (key_len_min, key_len_max);
        return rand.get_string (len, symb_begin, symb_end);
    };

    const std::size_t N_max = 1'000'000;
    db::data_base_t db_rel;
    db_native::data_base_t db_ref;

    int N = 0;
    for (; N < N_max / 2; ++N) {
        const auto key = gen_str ();
        ASSERT_EQ (db_rel.insert (key, "lol"), db_ref.insert (key, "lol"));
    }

    // insert, remove, update, find
    for (; N < N_max; ++N) {
        const auto key = gen_str ();

        auto type = rand.get_rand_val <unsigned> () % 4;
        switch (type) {
            case 0: {
                ASSERT_EQ (db_rel.insert (key, "lol"), db_ref.insert (key, "lol"))
                    << "key: " << key;
            } break;
            case 1: {
                ASSERT_EQ (db_rel.remove (key), db_ref.remove (key))
                    << "key: " << key;
            } break;
            case 2: {
                ASSERT_EQ (db_rel.update (key, "new lol"), db_ref.update (key, "new lol"))
                    << "key: " << key << "\n";
            } break;
            case 3: {
                auto it_rel = db_rel.find (key);
                auto it_ref = db_ref.find (key);

                if (it_ref == db_ref.end ()) {
                    ASSERT_EQ (it_rel, db_rel.end ())
                        << "key: " << key;
                } else {
                    ASSERT_NE (it_rel, db_rel.end ())
                        << "key: " << key;
                    ASSERT_EQ (it_rel->first, it_ref->first)
                        << it_rel->first.size << " " << key << " " << it_ref->second;
                }
            } break;
            default:
                throw std::runtime_error ("default!!!");
                break;
        }
    }
}