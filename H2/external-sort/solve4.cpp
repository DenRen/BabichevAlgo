// За 60 мин на C++
// Использовать максимум 3 файла

#include <cassert>

#include "solve.hpp"
#include "debug_func.h"
#include "../../libs/print_lib.hpp"

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#if 1
    #define dump(obj) std::cout << #obj ": " << obj << std::endl
#else
    #define dump(obj)
#endif
/*

Улучшения:
    Использовать в качестве хранителя лент только один файл и только с ним производить все вычисления,
    потому что std::remove стоит дорого
    Использовать собственный буффер и записывать и читать файл как можно реже
        Есть два варианта добиться этого:
            Работать непосредственно с I/O и вероятно опускаться на низкий уровень
            Дать шанс стандартной библиотеке работать с файлами.

            Здесь по сути можно поэкспериментировать
*/

std::vector <std::string>
read_chunk (unsigned max_ram_size,
            unsigned max_str_len,
            std::istream& is)
{
    std::vector <std::string> strs;

    unsigned size = 0;
    while (size < max_ram_size - max_str_len && !is.eof ()) {
        std::string temp;
        std::getline (is, temp);
        if (temp.size () == 0) {
            continue;
        }
        size += temp.size ();
        strs.emplace_back (std::move (temp));
    }

    return strs;
}

std::vector <std::size_t>
read_chunks_to_files (unsigned max_ram_size,
                      unsigned max_str_len,
                      std::istream& is,
                      std::ostream& os)
{
    std::vector <std::size_t> chunk_sizes;
    std::vector <std::string> strs;
    while (!is.eof ()) {
        strs = read_chunk (max_ram_size, max_str_len, is);
        if (!strs.size ()) {
            break;
        }

        std::sort (strs.begin (), strs.end ());

        std::size_t sum_len = 0;
        for (const auto& str : strs) {
            os << str << "\n";
            sum_len += str.size () + 1;
        }

        chunk_sizes.push_back (sum_len);
        strs.clear ();
    }

    return chunk_sizes;
}

void
get_line (std::istream& is,
          ssize_t& pos,
          const ssize_t& pos_end,
          std::string& str)
{
    if (pos >= pos_end) {
        return;
    }

    is.clear ();

    is.seekg (pos);
    is >> str;
    pos = 1 + is.tellg (); // + '\n'
}

void
merge (std::size_t chunk_size_l,
       std::size_t chunk_size_r,
       std::istream& is,
       std::ostream& os)
{
    ssize_t       pos_l     = is.tellg ();
    ssize_t const pos_l_end = pos_l + chunk_size_l;
    ssize_t       pos_r     = pos_l + chunk_size_l;
    ssize_t const pos_r_end = pos_r + chunk_size_r;
    std::string str_l, str_r;

    auto get_line_l = [&] () { get_line (is, pos_l, pos_l_end, str_l); };
    auto get_line_r = [&] () { get_line (is, pos_r, pos_r_end, str_r); };

    get_line_l ();
    get_line_r ();
    while (chunk_size_l && chunk_size_r) {
        if (str_l < str_r) {
            os << str_l << "\n";
            chunk_size_l -= (str_l.size () + 1);
            if (chunk_size_l) {
                get_line_l ();
            }
        } else {
            os << str_r << "\n";
            chunk_size_r -= (str_r.size () + 1);
            if (chunk_size_r) {
                get_line_r ();
            }
        }
    }

    if (chunk_size_l) {
        os << str_l << "\n";
        while (chunk_size_l -= (str_l.size () + 1)) {
            get_line_l ();
            os << str_l << "\n";
        }
    } else {
        os << str_r << "\n";
        while (chunk_size_r -= (str_r.size () + 1)) {
            get_line_r ();
            os << str_r << "\n";
        }
    }

    is.seekg (pos_r_end);
}

bool
merge_chunks (std::iostream& stream_from,  // Here places chuncks
              std::iostream& stream_to,
              std::vector <std::size_t>& chunk_sizes)
{
    stream_from.clear ();   // Delete EOF bit
    stream_to.clear ();
    stream_from.seekg (0);
    stream_to.seekp (0);

    int j = 0;
    for (int i = 1; i < chunk_sizes.size (); i += 2) {
        const auto& chunk_size_l = chunk_sizes[i - 1];
        const auto& chunk_size_r = chunk_sizes[i];

        merge (chunk_size_l, chunk_size_r, stream_from, stream_to);
        chunk_sizes[j++] = chunk_size_l + chunk_size_r;
    }

    if (chunk_sizes.size () % 2 != 0) {
        auto size = chunk_sizes[chunk_sizes.size () - 1];
        chunk_sizes[j++] = size;
        std::string str;

        ++size;
        while (size -= (str.size () + 1)) {
            stream_from >> str;
            stream_to << str << "\n";
        }
    }

    chunk_sizes.resize (j);
    if (chunk_sizes.size () == 1) {
        return false;
    }

    return !merge_chunks (stream_to, stream_from, chunk_sizes);
}

void
create_file (const std::string& name) {
    std::fstream fs {name, std::ios_base::out};
}

void
test_read_chunsk (unsigned max_ram_size,
                  unsigned max_str_len,
                  std::string input_file_name,
                  std::string output_file_name);

void
solve (unsigned max_ram_size,
       unsigned max_str_len,
       std::string input_file_name,
       std::string output_file_name)
{
    const std::string tmp_file_name = "tmp.txt";

    create_file (output_file_name);
    create_file (tmp_file_name);

    std::fstream is {input_file_name};
    std::fstream os {output_file_name};
    std::fstream ts {tmp_file_name};

    auto chunk_sizes = read_chunks_to_files (max_ram_size, max_str_len, is, os);

    bool res = merge_chunks (os, ts, chunk_sizes);
    if (res) {
        std::remove (tmp_file_name.data ());
    } else {
        std::remove (output_file_name.data ());
        rename (tmp_file_name.data (), output_file_name.data ());
    }
}

void
test_read_chunsk (unsigned max_ram_size,
                  unsigned max_str_len,
                  std::string input_file_name,
                  std::string output_file_name)
{
    create_file (output_file_name);

    std::fstream is {input_file_name};
    std::fstream os {output_file_name};

    auto chunk_sizes = read_chunks_to_files (max_ram_size, max_str_len, is, os);

    is.clear ();
    os.clear ();

    is.seekg (0);
    os.seekg (0);

    for (auto chunk_size : chunk_sizes) {
        std::vector <std::string> strs_ref, strs;
        // dump (chunk_size);
        while (chunk_size) {
            std::string str_ref, str;
            std::getline (is, str_ref);
            std::getline (os, str);

            chunk_size -= (str_ref.size () + 1);

            strs_ref.push_back (std::move (str_ref));
            strs.push_back (std::move (str));
        }

        if (strs_ref.size () != strs.size ()) {
            throw std::runtime_error ("err");
        }

        std::sort (strs_ref.begin (), strs_ref.end ());

        for (int i = 0; i < strs_ref.size (); ++i) {
            if (strs[i] != strs_ref[i]) {
                std::cout << "F\n";
            }
        }

    }
}