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

/*
TODO:
    1. define NDEBUG
    2. opt fseek
    3. write cache
*/

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

namespace db {

class data_base_t {
    typedef std::size_t hash_t;
    typedef uint32_t size_t;
    typedef uint32_t num_page_t;

    const unsigned page_size = 4096;
    const unsigned kv_page_size = 2 * page_size;

    struct key_t {
        hash_t hash;
        size_t size;

        key_t (hash_t hash, size_t size) :
            hash (hash), size (size)
        {}

        bool
        operator < (const data_base_t::key_t& rhs) const noexcept {
            return hash != rhs.hash ?
                        hash < rhs.hash :
                        size < rhs.size;
        }
        bool
        operator == (const data_base_t::key_t& rhs) const noexcept {
            return (hash == rhs.hash) && (size == rhs.size);
        }

        bool
        operator == (const std::string& str) const noexcept {
            return hash != std::hash <std::string> {} (str) ?
                       false : size == str.size ();
        }
    };

    struct page_t {
        size_t size;
        num_page_t num_page;

        bool
        operator == (const data_base_t::page_t& rhs) const noexcept {
            return num_page == rhs.num_page;
        }
    };

private:
    std::multimap <key_t, page_t> page_table;
    std::queue <num_page_t> empty_pages;

    mutable FILE* db_file = nullptr;
    long num_kv_page = 0;
    mutable uint8_t* buf = new uint8_t[kv_page_size];

    mutable bool is_last_num_page = true;

    key_t
    calc_key_hash (const std::string& str) const {
        key_t key {std::hash <std::string> {} (str),
                   static_cast <size_t> (str.size ())};
        return key;
    }

    num_page_t
    write_new_page (const std::string& key_str,
                    const std::string& val_str) {
        const auto key_size = key_str.size ();
        const auto val_size = val_str.size ();
        const auto size = key_size + val_size;

        std::copy (key_str.data (), key_str.data () + key_size, buf);
        std::copy (val_str.data (), val_str.data () + val_size, buf + key_size);

        if (empty_pages.size () == 0) {
            if (!is_last_num_page) {
                const uint64_t cur_pos = (uint64_t) kv_page_size * num_kv_page;
                if (fseek (db_file, cur_pos, SEEK_SET) == -1) {
                    throw std::system_error (errno, std::generic_category (), "fseek");
                }
                is_last_num_page = true;
            }

            if (fwrite_unlocked (buf, kv_page_size, 1, db_file) != 1) {
                throw std::system_error (errno, std::generic_category (), "fwrite");
            }


            return num_kv_page++;
        } else {
            const auto num_page = empty_pages.front ();
            const uint64_t write_pos = (uint64_t) kv_page_size * num_page;
            empty_pages.pop ();

            if (fseek (db_file, write_pos,  SEEK_SET) == -1) {
                throw std::system_error (errno, std::generic_category (), "fseek");
            }

            is_last_num_page = false;

            if (fwrite_unlocked (buf, kv_page_size, 1, db_file) != 1) {
                throw std::system_error (errno, std::generic_category (), "fwrite");
            }

            return num_page;
        }
    }

    void
    update_page (const key_t& key,
                 const page_t& page,
                 const std::string& val_str) {
        const uint64_t cur_pos   = (uint64_t) kv_page_size * num_kv_page;
        const uint64_t write_pos = (uint64_t) kv_page_size * page.num_page + key.size;

        if (fseek (db_file, write_pos,  SEEK_SET) == -1) {
            throw std::system_error (errno, std::generic_category (), "fseek");
        }

        if (fwrite_unlocked (val_str.data (), val_str.size (), 1, db_file) != 1) {
            throw std::system_error (errno, std::generic_category (), "fwrite");
        }
        
        is_last_num_page = false;
    }

    void
    set_buf_zero_term (const key_t& key, const page_t& page) const {
        buf[key.size + page.size] = '\0';
    }

    void
    read_page (const key_t& key, const page_t& page) const {
        const uint64_t read_pos = (uint64_t) kv_page_size * page.num_page;

        if (fseek (db_file, read_pos, SEEK_SET) == -1) {
            throw std::system_error (errno, std::generic_category (), "fseek");
        }

        if (fread_unlocked (buf, 1, kv_page_size, db_file) != kv_page_size) {
            throw std::system_error (errno, std::generic_category (), "fread");
        }
        
        is_last_num_page = false;
    }

    // key and buf_key has same sizes
    bool
    compare_str_with_buf_key (const std::string& key) const {
        const auto size = key.size ();
        return std::equal (key.data (), key.data () + size, buf);
    }

    // If good then K,V will in buf
    decltype (page_table)::iterator
    find (const std::string& key_str, key_t key, decltype (page_table)::iterator it_end) {
        for (auto it_key = page_table.find (key);
             it_key != it_end && it_key->first == key;
             ++it_key)
        {
            const key_t& cur_key = it_key->first;
            const page_t& cur_page = it_key->second;

            read_page (cur_key, cur_page);
            bool is_equal_key = compare_str_with_buf_key (key_str);
            if (is_equal_key) {
                return it_key;
            }
        }

        return it_end;
    }

    std::ostream&
    dump_buf_impl (std::ostream& os = std::cout) const {
        std::ostream_iterator <char> it {os};
        std::copy_n (buf, kv_page_size, it);
        return os << "\n";
    }

public:
    data_base_t (std::string name_db_file = "db.txt") :
        db_file (fopen (name_db_file.data (), "wb+"))
    {
        if (db_file == nullptr) {
            std::string err_msg = "Failed to create db file: ";
            throw std::runtime_error (err_msg + name_db_file);
        }
    }

    ~data_base_t () {
        fclose (db_file);
        delete[] buf;
    }

    decltype (page_table)::const_iterator
    end () const {
        return page_table.end ();
    }

    decltype (page_table)::iterator
    find (const std::string& key_str) {
        key_t key = calc_key_hash (key_str);
        auto it_end = page_table.end ();
        return find (key_str, key, it_end);
    }

    bool
    insert (const std::string& key_str,
            const std::string& value_str) {
        key_t key = calc_key_hash (key_str);
        auto it_end = page_table.end ();
        if (find (key_str, key, it_end) != it_end) {
            return false;
        }

        // Key missing => add new page
        page_t page;
        page.num_page = write_new_page (key_str, value_str);
        page.size = value_str.size ();

        page_table.insert ({key, page});
        return true;
    }

    bool
    update (const std::string& key_str,
            const std::string& value_str) {
        key_t key = calc_key_hash (key_str);

        auto it_end = page_table.end ();
        auto it = find (key_str, key, it_end);
        if (it == it_end) {
            return false;
        }

        const auto& page = it->second;
        update_page (key, page, value_str);
        it->second.size = value_str.size ();

        return true;
    }

    bool
    remove (const std::string& key_str) {
        key_t key = calc_key_hash (key_str);

        auto it_end = page_table.end ();
        auto it = find (key_str, key, it_end);
        if (it == it_end) {
            return false;
        }

        empty_pages.push (it->second.num_page);
        page_table.erase (it);

        return true;
    }

    bool
    print (const std::string& key_str, std::string& out) {
        key_t key = calc_key_hash (key_str);
        auto it_end = page_table.end ();

        auto it = find (key_str, key, it_end);
        if (it == it_end) {
            return false;
        }

        const page_t& page = it->second;
        const char* buf_from = (const char*) buf + key.size;
        out.insert (0, buf_from, page.size);
        out.resize (page.size);

        return true;
    }
}; // class data_base_t

} // namespace db

namespace db_native {

class data_base_t {
    std::map <std::string, std::string> map;

    void
    check_key (const std::string& key) const {
    #ifndef NDEBUG
        if (key.size () == 0) {
            throw std::runtime_error ("key couldn't have size 0");
        } else if (key.size () > 4096) {
            throw std::runtime_error ("key couldn't have size more 4096");
        }
    #endif
    }

public:
    decltype (map)::iterator
    end () {
        return map.end ();
    }

    decltype (map)::iterator
    find (const std::string& key) {
        check_key (key);

        return map.find (key);
    }

    bool
    insert (const std::string& key_str,
            const std::string& value_str) {
        check_key (key_str);

        auto it = map.find (key_str);
        if (it != map.end ()) {
            return false;
        }

        map.insert ({key_str, value_str});
        return true;
    }

    bool
    update (const std::string& key_str,
            const std::string& value_str) {
        check_key (key_str);

        auto it = map.find (key_str);
        if (it == map.end ()) {
            return false;
        }

        it->second = value_str;
        return true;
    }

    bool
    remove (const std::string& key_str) {
        check_key (key_str);

        auto it = map.find (key_str);
        if (it == map.end ()) {
            return false;
        }

        map.erase (it);
        return true;
    }

    bool
    print (const std::string& key_str, std::string& out) const {
        auto it = map.find (key_str);
        if (it == map.end ()) {
            return false;
        }

        out = it->second;
        return true;
    }
}; // class data_base_t

} // namespace db_native

std::size_t
read_word (std::string& src,
           std::size_t begin,
           std::string& dest)
{
    std::size_t end = src.find (' ', begin);
    if (end == std::string::npos) {
        end = src.size ();
    }

    const char* begin_ptr = src.data () + begin;
    const std::size_t size = end - begin;

    dest.insert (0, begin_ptr, size);
    dest.resize (size);

    return end + 1;
}

void
solve (std::size_t num_req,
       std::istream& is,
       std::ostream& os,
       std::size_t max_ram)
{
    db::data_base_t db;

    const int max_len = 4096;
    const int max_len_req = 20 + 2 * max_len + 1;

    std::string req, key, val, out;
    req.reserve (max_len_req);
    key.reserve (max_len + 1);
    val.reserve (max_len + 1);
    out.reserve (max_len + 1);

    auto print_error  = [&] (bool is_not_error) {
        if (!is_not_error) {
            os << "ERROR\n";
        }
    };

    std::getline (is, req);
    std::stringstream stream {req};
    for (std::size_t i_req = 0; i_req < num_req; ++i_req) {
        std::getline (is, req);

        auto begin = req.find (' ') + 1;
        auto next_word = read_word (req, begin, key);
        switch (req[0]) {
            case 'A': {
                read_word (req, next_word, val);
                print_error (db.insert (key, val));
            } break;
            case 'D': {
                print_error (db.remove (key));
            } break;
            case 'U': {
                read_word (req, next_word, val);
                print_error (db.update (key, val));
            } break;
            case 'P': {
                bool state = db.print (key, out);
                print_error (state);
                if (state) {
                    os << key << " " << out << "\n";
                }
            } break;
            default: {
                throw std::invalid_argument (req);
            }
        }
    }
}