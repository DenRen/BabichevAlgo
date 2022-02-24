#include <cassert>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <streambuf>
#include <ext/stdio_filebuf.h>

#include "solve.hpp"
#include "debug_func.h"
#include "../../libs/print_lib.hpp"

class fast_file {
    int fd_;
    __gnu_cxx::stdio_filebuf <char> buf_;
    std::ostream os_;

    int
    open_file (const std::string& name) {
        int fd = open (name.data (), O_WRONLY | O_CREAT, 0666);
        if (fd == -1) {
            throw std::runtime_error ("Failed open file");
        }

        return fd;
    }

public:
    fast_file (const std::string& name) :
        fd_ {open_file (name)},
        buf_ {fd_, std::ios_base::out},
        os_ {&buf_}
    {}

    std::ostream&
    get_stream () {
        return os_;
    }

    void
    cut () {
        if (ftruncate (fd_, get_cur_pos ()) == -1) {
            throw std::runtime_error ("ftruncate error");
        }
    }

    std::size_t
    get_cur_pos () const {
        long cur_pos = lseek64 (fd_, 0, SEEK_CUR);
        if (cur_pos == -1) {
            std::runtime_error ("lseek64 error");
        }
        return cur_pos;
    }
};

std::pair <std::vector <std::string>,
           std::size_t>
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

    return std::pair {std::move (strs), size};
}

std::vector <std::size_t>
read_and_sort_chunks (unsigned max_ram_size,
                      unsigned max_str_len,
                      const std::string& input_file_name,
                      std::fstream& fs)
{
    std::ifstream input {input_file_name};

    std::vector <std::size_t> chunk_sizes;
    while (!input.eof ()) {
        auto[strs, size] = read_chunk (max_ram_size, max_str_len, input);
        std::sort (strs.begin (), strs.end ());

        for (const auto& str : strs) {
            fs << str << "\n";
        }
        std::flush (fs);

        if (fs.fail ()) {
            throw std::runtime_error ("Write to std::fstream error");
        }
        fs.clear ();

        chunk_sizes.push_back (size);
    }

    return chunk_sizes;
}

void
merge_two_chunks (unsigned max_ram_size,
                  unsigned max_str_len,
                  std::vector <std::size_t>& chunk_sizes,
                  std::fstream& f)
{
    
}

void
merge_chunks (unsigned max_ram_size,
              unsigned max_str_len,
              std::vector <std::size_t>& chunk_sizes,
              std::fstream& fs)
{
    std::vector <std::size_t> new_chunk_size;
    new_chunk_size.reserve (chunk_sizes.size () / 2 + chunk_sizes.size () % 2);

    for (std::size_t i = 1; i < chunk_sizes.size (); i += 2) {
        const auto& size_1 = chunk_sizes[i - 1];
        const auto& size_2 = chunk_sizes[i];

        new_chunk_size.push_back (size_1 + size_2);


    }
}

std::fstream
open_file (const std::string& name)
{
    std::fstream fs;

    auto try_open = [&name, &fs] () {
        fs.open (name.data (), std::ios_base::out | std::ios_base::in);
    };

    try_open ();
    if (fs.fail ()) {
        fs.open (name.data (), std::ios_base::out);
        fs.close ();

        try_open ();
        if (fs.fail ()) {
            throw std::runtime_error ("Failed to open/create file");
        }
    }

    return fs;
}

void
solve (unsigned max_ram_size,
       unsigned max_str_len,
       std::string input_file_name,
       std::string output_file_name)
{
    std::fstream fs = open_file (output_file_name); // Tape file

    auto chunk_sizes = read_and_sort_chunks (max_ram_size, max_str_len,
                                             input_file_name, fs);
    fs.seekp (0);
    merge_chunks (max_ram_size, max_str_len, chunk_sizes, fs);
}