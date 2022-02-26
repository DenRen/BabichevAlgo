#include <cassert>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <streambuf>
#include <cstring>

#include <sys/stat.h>

#include "../../libs/print_lib.hpp"

void
throw_runtime_error (const std::string_view& msg)
{
    std::cerr << std::strerror (errno) << std::endl;
    throw std::runtime_error (msg.data ());
}

int
open_file (const std::string& name, int flags)
{
    if (int fd = open (name.data (), flags, 0666); fd != -1) {
        return fd;
    }

    throw_runtime_error ("Failed to open file");
    return -1;
}

int
get_block_size (int fd)
{
    if (struct stat stat; fstat (fd, &stat) != -1) {
        return stat.st_blksize;
    }

    throw_runtime_error ("Failed to get fstat");
    return -1;
}

template <typename T>
T*
get_aligned_mem (std::size_t alignment,
                 std::size_t size)
{
    if (T* ptr = (T*) aligned_alloc (alignment, size * sizeof (T));
        ptr != nullptr) {
        return ptr;
    }

    throw_runtime_error ("Failed to allocate aligned memory");
    return nullptr;
}

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

void
sure_write (int fd,
            const char* data,
            std::size_t size)
{
    ssize_t writen_size = 0;
    while ((writen_size = write (fd, data, size)) != size) {
        if (writen_size == -1) {
            throw_runtime_error ("Failed write");
        }

        data += writen_size;
        size -= writen_size;
    }
}

std::vector <std::size_t>
read_and_sort_chunks (int fd_out,
                      const std::string& input_file_name,
                      char* const buffer,
                      const std::size_t buffer_size)
{
    int fd_in = open_file (input_file_name, O_RDONLY);
    std::vector <std::size_t> chunk_sizes;
    std::vector <std::string_view> strs;

    ssize_t read_size = 0, filled_size = 0;
    while ((read_size = read (fd_in, buffer + filled_size, buffer_size - filled_size))) {
        if (read_size == -1) {
            throw_runtime_error ("Failed to read");
        } else if (read_size == 0) {
            break;
        }

        char* cur = buffer, *end = buffer + filled_size + read_size;
        char* str_end = nullptr;
        while ((str_end = std::find (cur, end, '\n')) != end) {
            strs.emplace_back (cur, str_end - cur);
            cur = str_end + 1;
        }

        // Sort and write chunk to tape
        std::sort (strs.begin (), strs.end ());

        std::size_t chunk_size = 0;
        for (const auto& str : strs) {
            sure_write (fd_out, str.data (), str.size () + 1);
            chunk_size += str.size () + 1;
        }
        chunk_sizes.push_back (chunk_size);

        strs.clear ();

        std::copy (cur, str_end, buffer);
        filled_size = str_end - cur;
    }

    close (fd_in);

    return chunk_sizes;
}

char*
find_end (char* data,
          unsigned size,
          char symb)
{
    std::reverse_iterator <char*> rbegin {data + size};
    std::reverse_iterator <char*> rend {data};

    return std::find (rbegin, rend, symb).base ();
}

std::pair <unsigned,    // left
           unsigned>    // right
merge_2_buffers (int fd_out,
                 char* buf_l,
                 char* buf_r,
                 unsigned buf_l_size,
                 unsigned buf_r_size)
{
    char* const buf_l_begin = buf_l;
    char* const buf_r_begin = buf_r;
    char* const buf_l_real_end = buf_l_begin + buf_l_size;
    char* const buf_r_real_end = buf_r_begin + buf_r_size;

    char* buf_l_end = find_end (buf_l, buf_l_size, '\n');
    char* buf_r_end = find_end (buf_r, buf_r_size, '\n');

    if (buf_l_end == buf_l) {
        buf_l_end = buf_l_real_end;
    }
    if (buf_r_end == buf_r) {
        buf_r_end = buf_r_real_end;
    }

    std::string_view str_l {buf_l, std::find (buf_l, buf_l_end, '\n') - buf_l};
    std::string_view str_r {buf_r, std::find (buf_r, buf_r_end, '\n') - buf_r};

    std::cout << "str_l.size (): " << str_l.size () << std::endl;
    std::cout << "str_r.size (): " << str_r.size () << std::endl;

    while (str_l.size () || str_r.size ()) {
        bool write_left;

        if (str_l.size () == 0 || str_r.size () == 0) {
            if (str_l.size () == 0) {
                write_left = false;
            } else {
                write_left = true;
            }
        } else {
            write_left = str_l < str_r;
        }

        if (write_left) {
            const auto len = str_l.size () + 1;         // + size ('\n')
            sure_write (fd_out, str_l.data (), len);

            std::cout << "l len: " << len << std::endl;

            buf_l += len;
            if (buf_l >= buf_l_end) {
                buf_l = buf_l_real_end;
                std::string_view empty_str {"", 0};
                std::swap (str_l, empty_str);
                continue;
            }

            const auto new_str_len = std::find (buf_l, buf_l_end, '\n') - buf_l;
            std::string_view new_str {buf_l, new_str_len};
            std::swap (str_l, new_str);
        } else {
            const auto len = str_r.size () + 1;         // + size ('\n')
            sure_write (fd_out, str_r.data (), len);

            std::cout << "r len: " << len << std::endl;

            buf_r += len;
            if (buf_r >= buf_r_end) {
                buf_r = buf_r_real_end;
                std::string_view empty_str {"", 0};
                std::swap (str_r, empty_str);
                continue;
            }

            const ssize_t new_str_len = std::find (buf_r, buf_r_end, '\n') - buf_r;
            std::cout << "new_str_len: " << new_str_len << std::endl;
            std::cout << "buf_r_end - buf_r: " << buf_r_end - buf_r << std::endl;
            std::string_view new_str {buf_r, new_str_len};
            std::swap (str_r, new_str);
        }
    }

    // if (!str_l.size () || !str_r.size ()) {
    //     sleep (1);
    // }

    // Copy other data to begin
    char* t1 = std::copy (buf_l, buf_l_real_end, buf_l_begin);
    char* t2 = std::copy (buf_r, buf_r_real_end, buf_r_begin);

    // memset (t1, 0, buf_l_real_end - t1);
    // memset (t2, 0, buf_l_real_end - t2);

    return {buf_l_real_end - buf_l,
            buf_r_real_end - buf_r};
}

void
merge_2_chunks (int fd_from,
                int fd_to,
                std::size_t chunk_size_l,
                std::size_t chunk_size_r,
                char* const buffer,
                const std::size_t buffer_size)
{
    char* const buf_l = buffer,
        * const buf_r = buffer + buffer_size / 2;
    std::size_t buf_l_size = buf_r - buf_l,
                buf_r_size = buffer_size - buf_l_size;

    ssize_t read_size_l = buf_l_size,
            read_size_r = buf_r_size,
            fd_pos_l = 0,
            fd_pos_r = chunk_size_l;

    int buf_l_filled_size = 0,
        buf_r_filled_size = 0;

    while (chunk_size_l || chunk_size_r) {
        // Read data to left buffer
        lseek (fd_from, fd_pos_l, SEEK_SET);
        read_size_l = read (fd_from, buf_l + buf_l_filled_size, read_size_l);
        if (read_size_l < 0) {
            throw_runtime_error ("Failed to read left part");
        }
        chunk_size_l -= read_size_l;
        fd_pos_l = lseek (fd_from, 0, SEEK_CUR);
        std::cout << "read_size_l: " << read_size_l << std::endl;

        // Read data to right buffer
        lseek (fd_from, fd_pos_r, SEEK_SET);
        read_size_r = read (fd_from, buf_r + buf_r_filled_size, read_size_r);
        if (read_size_r < 0) {
            throw_runtime_error ("Failed to read right part");
        }
        chunk_size_r -= read_size_r;
        fd_pos_r = lseek (fd_from, 0, SEEK_CUR);
        std::cout << "read_size_r: " << read_size_r << std::endl;

        // Merge both buffers to file
        std::tie (buf_l_filled_size, buf_r_filled_size) =
        merge_2_buffers (fd_to, buf_l, buf_r,
                         read_size_l + buf_l_filled_size,
                         read_size_r + buf_r_filled_size);

        
        std::cout << "chunk_size_l: " << chunk_size_l << std::endl;
        std::cout << "chunk_size_r: " << chunk_size_r << std::endl;
        std::cout << "buf_l_filled_size: " << buf_l_filled_size << std::endl;
        std::cout << "buf_r_filled_size: " << buf_r_filled_size << std::endl;

        std::cout << "read_size_l: " << read_size_l << std::endl;
        std::cout << "read_size_r: " << read_size_r << std::endl;
        // Calc already readed data sizes
        read_size_l = std::min (buf_l_size - buf_l_filled_size, chunk_size_l);
        read_size_r = std::min (buf_r_size - buf_r_filled_size, chunk_size_r);
        std::cout << "read_size_l: " << read_size_l << std::endl;
        std::cout << "read_size_r: " << read_size_r << std::endl << std::endl;
    }
}

int
merge_chunks (int fd,
              int fd_tmp,
              std::vector <std::size_t>& chunks_size,
              char* const buffer,
              std::size_t buffer_size)
{
    int fd_save = fd;
    int fd_tmp_save = fd_tmp;

    int& fd_from = fd_tmp;  // See swap
    int& fd_to = fd;

    while (chunks_size.size () != 1) {
        std::swap (fd_from, fd_to);
        lseek (fd,     0, SEEK_SET);
        lseek (fd_tmp, 0, SEEK_SET);

        int pos_chunk_size = 0;
        for (std::size_t i = 1; i < chunks_size.size (); i += 2) {
            const auto& size_l = chunks_size[i - 1],
                      & size_r = chunks_size[i];

            // memset (buffer, '\0', buffer_size);
            merge_2_chunks (fd_from, fd_to, size_l, size_r, buffer, buffer_size);
            chunks_size[pos_chunk_size++] = size_l + size_r;
        }

        if (chunks_size.size () % 2 != 0) {
            const auto size = chunks_size[chunks_size.size () - 1];
            splice (fd_from, nullptr, fd_to, nullptr, size, 0);
            chunks_size[pos_chunk_size++] = size;
        }

        chunks_size.resize (pos_chunk_size);
    }

    return fd_to;
}

void
solve (unsigned max_ram_size,
       unsigned max_str_len,
       std::string input_file_name,
       std::string output_file_name)
{
    int fd = open_file (output_file_name, O_RDWR | O_CREAT); // Tape file

    const std::size_t buffer_size = max_ram_size - max_str_len;
    char* buffer = get_aligned_mem <char> (get_block_size (fd), buffer_size);

    auto chunk_sizes = read_and_sort_chunks (fd, input_file_name,
                                             buffer, buffer_size);

    const std::string_view tmp_file_name = "tmp.txt";
    int fd_tmp = open_file (tmp_file_name.data (), O_RDWR | O_CREAT);
    int fd_out = merge_chunks (fd, fd_tmp, chunk_sizes, buffer, buffer_size);

    ftruncate (fd_out, chunk_sizes[0]);

    close (fd_tmp);
    close (fd);

    if (fd_out == fd) {
        std::remove (tmp_file_name.data ());
    } else {
        std::remove (output_file_name.data ());
        std::rename (tmp_file_name.data (), output_file_name.data ());
    }

    free (buffer);
}