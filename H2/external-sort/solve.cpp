#include <cassert>

#include "solve.hpp"
#include "debug_func.h"
#include "../../libs/print_lib.hpp"

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

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

long
get_unread_size (FILE* file) {
    long begin = ftell (file);
    CHECK (begin != -1);

    CHECK_ERR (fseek (file, 0, SEEK_END));
    long end = ftell (file);
    CHECK (end);

    CHECK_ERR (fseek (file, begin, SEEK_SET));

    return end - begin;
}

std::vector <std::string>
read_chunk (unsigned max_ram_size,
            unsigned max_str_len,
            std::ifstream& is)
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

class rand_name {
    int begin_;

public:
    rand_name (int seed = 0) :
        begin_ (seed)
    {}

    std::string
    gen () {
        const std::string base = "tmp_";
        return  base + std::to_string (begin_++);
    }
};

struct file_info_t : public std::pair <std::string, unsigned> {
    std::string&
    name_file () noexcept {
        return first;
    }

    const std::string&
    name_file () const noexcept {
        return first;
    }

    unsigned
    num_lines () const {
        return second;
    }

    file_info_t () = default;

    template <typename T>
    file_info_t (T&& name_file, unsigned num_lines) :
        std::pair <std::string, unsigned> {std::forward <T> (name_file), num_lines}
    {}

    file_info_t (const file_info_t&) = default;
    file_info_t (file_info_t&&) = default;

    file_info_t& operator = (const file_info_t&) = default;
    file_info_t& operator = (file_info_t&&) = default;
};

std::vector <file_info_t>
read_chunks_to_files (unsigned max_ram_size,
                      unsigned max_str_len,
                      const std::string input_file_name,
                      rand_name& name_gen)
{
    std::ifstream input {input_file_name};

    std::vector <file_info_t> chunk_file_names;
    while (!input.eof ()) {
        std::vector <std::string> strs = read_chunk (max_ram_size,
                                                     max_str_len, input);
                                                     
        std::sort (strs.begin (), strs.end ());

        std::string chunk_file_name = name_gen.gen ();
        std::ofstream os {chunk_file_name};

        for (const auto& str : strs) {
            os << str << std::endl;
            // std::cout << "\"" << str << "\"" << std::endl;
        }

        chunk_file_names.emplace_back (std::move (chunk_file_name), strs.size ());
    }

    return chunk_file_names;
}

int
remove_files (const std::vector <file_info_t>& file_names)
{
    for (const auto& file_info : file_names) {
        CHECK_ERR (std::remove (file_info.name_file ().data ()));
    }

    return 0;
}

void
merge (const file_info_t& file_info_1,
       const file_info_t& file_info_2,
       std::ostream& os)
{
    std::ifstream is1 {file_info_1.name_file ()},
                  is2 {file_info_2.name_file ()};
    unsigned num_lines_1 = file_info_1.num_lines (),
             num_lines_2 = file_info_2.num_lines ();

    std::string str1, str2;
    is1 >> str1;
    is2 >> str2;
    while (num_lines_1 && num_lines_2) {
        if (str1 < str2) {
            os << str1 << "\n";
            if (--num_lines_1) {
                is1 >> str1;
            }
        } else {
            os << str2 << "\n";
            if (--num_lines_2) {
                is2 >> str2;
            }
        }
    }

    if (num_lines_1) {
        os << str1 << "\n";
    } else {
        os << str2 << "\n";
    }

    std::istream& is = num_lines_1 ? is1 : is2;
    auto tail_size = num_lines_1 ? num_lines_1 : num_lines_2;

    std::string s;
    while (--tail_size) {
        is >> s;
        os << s << "\n";
    }
}

int
merge_chunks (const std::string& output_file_name,
              std::vector <file_info_t>& chunk_files,
              rand_name& name_gen)
{
    // std::cout << "merge chunks, files: " << chunk_files << std::endl;

    const auto num_files = chunk_files.size ();
    if (num_files == 1) {
        CHECK_ERR (std::rename (chunk_files[0].name_file ().data (),
                                output_file_name.data ()));
        return 0;
    }

    std::vector <file_info_t> new_chunk_files;
    unsigned i = 0;
    for (; i < (num_files & (~1ull)); i += 2) {
        std::string chunk_name = name_gen.gen ();
        std::ofstream os {chunk_name};

        merge (chunk_files[i], chunk_files[i+1], os);

        unsigned num_lines = chunk_files[i].num_lines () + chunk_files[i+1].num_lines ();

        new_chunk_files.emplace_back (std::move (chunk_name), num_lines);
    }

    file_info_t last_file_info;
    if (i != num_files) {
        last_file_info = std::move (chunk_files[num_files - 1]);
        chunk_files.pop_back ();
    }

    remove_files (chunk_files);

    chunk_files = std::move (new_chunk_files);
    if (i != num_files) {
        chunk_files.emplace_back (std::move (last_file_info));
    }

    return merge_chunks (output_file_name, chunk_files, name_gen);
}

void
solve (unsigned max_ram_size,
       unsigned max_str_len,
       std::string input_file_name,
       std::string output_file_name)
{
    rand_name name_gen;
    auto chunk_files = read_chunks_to_files (max_ram_size, max_str_len,
                                             input_file_name, name_gen);

    if (merge_chunks (output_file_name, chunk_files, name_gen) == -1) {
        std::runtime_error ("merge_chunks error");
    }
}