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
            auto key_str = std::to_string (key);
            auto it = tree.find (key_str);

            ASSERT_NE (it, tree.end ());
            ASSERT_EQ (it->first, key_str);
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

template <typename DB>
void
test_db_static_insert_print () {
    DB tree;
    const int max_size = 1000;
    const int key_begin = -500;

    std::string str_buf, value = "lol";

    int cur_key = key_begin;
    for (int size = 0; size <= max_size; ++size) {
        tree.insert (std::to_string (cur_key), value);

        for (int key = key_begin; key <= cur_key; ++key) {
            auto key_str = std::to_string (key);

            ASSERT_TRUE (tree.print (key_str, str_buf));
            ASSERT_EQ (str_buf, value) << key_str;
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
test_db_random_insert_print () {
    const int max_size = 1000;

    seclib::RandomGenerator rand;
    auto keys = rand.get_vector <int> (max_size);
    std::set <int> inserted_keys;

    std::string str_buf, value = "lol";

    DB tree;
    for (int size = 0; size < max_size; ++size) {
        auto key = std::to_string (keys[size]);

        tree.insert (key, value);
        inserted_keys.insert (keys[size]);

        for (int i = 0; i <= size; ++i) {
            auto key = std::to_string (keys[i]);

            ASSERT_TRUE (tree.print (key, str_buf));
            ASSERT_EQ (value, str_buf);
        }

        for (int i = size + 1; i < max_size; ++i) {
            if (inserted_keys.find (keys[i]) == inserted_keys.end ()) {
                ASSERT_FALSE (tree.print (std::to_string (keys[i]), str_buf));
            }
        }
    }
}

TEST (NATIVE_DB_STATIC,  INSERT_FIND)      { test_db_static_insert_find      <db_native::data_base_t> (); }
TEST (NATIVE_DB_RANDOM,  INSERT_FIND_UNIQ) { test_db_static_insert_find_uniq <db_native::data_base_t> (); }
TEST (NATIVE_DB_RANDOM,  INSERT_FIND)      { test_db_static_insert_find_uniq <db_native::data_base_t> (); }
TEST (NATIVE_DB_RANDOM,  REMOVE)           { test_db_random_remove           <db_native::data_base_t> (); }
TEST (NATIVE_DB_STATIC,  INSERT_PRINT)     { test_db_static_insert_print     <db_native::data_base_t> (); }
TEST (NATIVE_DB_RANDOM,  INSERT_PRINT)     { test_db_random_insert_print     <db_native::data_base_t> (); }

TEST (RELEASE_DB_STATIC, INSERT_FIND)      { test_db_static_insert_find      <db::data_base_t> (); }
TEST (RELEASE_DB_RANDOM, INSERT_FIND_UNIQ) { test_db_static_insert_find_uniq <db::data_base_t> (); }
TEST (RELEASE_DB_RANDOM, INSERT_FIND)      { test_db_static_insert_find_uniq <db::data_base_t> (); }
TEST (RELEASE_DB_RANDOM, REMOVE)           { test_db_random_remove           <db::data_base_t> (); }
TEST (RELEASE_DB_STATIC, INSERT_PRINT)     { test_db_static_insert_print     <db::data_base_t> (); }
TEST (RELEASE_DB_RANDOM, INSERT_PRINT)     { test_db_random_insert_print     <db::data_base_t> (); }

TEST (RELEASE_DB_USE_NATIVE, FULL) {
    const int key_len_min = 1, key_len_max = 4096;
    const char symb_begin = 'a', symb_end = 'z';

    seclib::RandomGenerator rand;

    auto gen_val = [&] () {
        const auto len = rand.get_rand_val <unsigned> (key_len_min, key_len_max);
        return rand.get_string (len, symb_begin, symb_end);
    };

    std::vector <std::string> saved_keys = {gen_val ()};    // Min one element
    auto gen_key = [&] () {
        if (rand.get_rand_val <unsigned> (10 + 1) >= 5 + 1) {
            saved_keys.push_back (gen_val ());
            return saved_keys[saved_keys.size () - 1];
        } else {
            const auto index = rand.get_rand_val <unsigned> (saved_keys.size ());
            return saved_keys[index];
        }
    };

    const std::size_t N_max = 1'000'000;
    db::data_base_t db_rel;
    db_native::data_base_t db_ref;

    int N = 0;
    for (; N < N_max / 2; ++N) {
        const auto key = gen_key (), val = gen_val ();
        ASSERT_EQ (db_rel.insert (key, val), db_ref.insert (key, val));
    }

    // rel -> short form of release
    // ref -> short form of reference
    std::string out_rel, out_ref;
    for (; N < N_max; ++N) {
        const auto key = gen_key ();

        // insert, remove, update, find
        auto type = rand.get_rand_val <unsigned> () % 4;
        switch (type) {
            case 0: {
                const auto val = gen_val ();
                ASSERT_EQ (db_rel.insert (key, val), db_ref.insert (key, val))
                    << "key: " << key;
            } break;
            case 1: {
                ASSERT_EQ (db_rel.remove (key), db_ref.remove (key))
                    << "key: " << key;
            } break;
            case 2: {
                // const auto new_val = gen_val ();
                // ASSERT_EQ (db_rel.update (key, new_val), db_ref.update (key, new_val))
                //     << "key: " << key << "\n";
            } break;
            case 3: {
                bool state_rel = db_rel.print (key, out_rel);
                bool state_ref = db_ref.print (key, out_ref);

                ASSERT_EQ (state_rel, state_ref);
                if (state_ref) {
                    ASSERT_EQ (out_rel, out_ref) << "key: " << key;
                }
            } break;
            default:
                throw std::runtime_error ("default!!!");
                break;
        }
    }
}