#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include "../../../libs/other_func.hpp"

#include <streambuf>
#include <ext/stdio_filebuf.h>

template <typename T>
double
to_ms (std::chrono::time_point <T> begin,
       std::chrono::time_point <T> end)
{
    return (end - begin).count () / 1000000.0;
}

using std::chrono::system_clock;

void
write_str_stream_fbuf (const std::string& str,
                       const std::string& name)
{
    // Если файла нет: 0.42
    auto begin = system_clock::now ();

    std::fstream fs;
    fs.open (name.data (), std::ios_base::out | std::ios_base::in);

    auto end = system_clock::now ();
    std::cout << "fs.open: " << to_ms (begin, end) << " ms" << std::endl;

    begin = system_clock::now ();

    fs.write (str.data (), str.size ());    

    end = system_clock::now ();
    std::cout << "fs.write: " << to_ms (begin, end) << " ms" << std::endl;

    // begin = system_clock::now ();
    // long pos = lseek64 (fd, 0, SEEK_CUR);
    // std::cout << pos << "\n";
    // std::cout << str.size () << "\n";
    // ftruncate (fd, pos);
    // end = system_clock::now ();
    // std::cout << "lseek64 & ftruncate: " << to_ms (begin, end) << " ms" << std::endl;
    

    begin = system_clock::now ();

    fs.seekg (0);

    char* read_buf = new char[str.size ()];

    fs.read (read_buf, str.size ());

    end = system_clock::now ();
    std::cout << "fs.read: " << to_ms (begin, end) << " ms" << std::endl;

    std::string_view str_read (read_buf, str.size ());
    if (str_read != str) {
        std::cout << "error" << std::endl;
    }

    delete read_buf;
}

void
write_str_stream_it (const std::string& str,
                     const std::string& name)
{
    // Если файла нет: 0.42
    auto begin = system_clock::now ();

    int fd = open (name.data (), O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        throw std::runtime_error ("open");
    }

    __gnu_cxx::stdio_filebuf <char> buf {fd, std::ios_base::out | std::ios_base::in};

    std::iostream os {&buf};
    auto end = system_clock::now ();
    std::cout << "ofstream ctor: " << to_ms (begin, end) << " ms" << std::endl;

    begin = system_clock::now ();
    os << str;
    std::flush (os);
    end = system_clock::now ();
    std::cout << "os << str: " << to_ms (begin, end) << " ms" << std::endl;

    // begin = system_clock::now ();
    // long pos = lseek64 (fd, 0, SEEK_CUR);
    // std::cout << pos << "\n";
    // std::cout << str.size () << "\n";
    // ftruncate (fd, pos);
    // end = system_clock::now ();
    // std::cout << "lseek64 & ftruncate: " << to_ms (begin, end) << " ms" << std::endl;
    
    std::ios::sync_with_stdio (false);

    os.seekp (0);
    begin = system_clock::now ();
    std::string str_read;
    str_read.reserve (str.size ());
    os >> str_read;
    end = system_clock::now ();
    std::cout << "os >> str: " << to_ms (begin, end) << " ms" << std::endl;

    if (str_read != str) {
        std::cout << "error" << std::endl;
    }
}

// 0.41
void
write_str_c (const std::string& str,
             const std::string& name)
{
    auto begin = system_clock::now ();

    int fd = open (name.data (), O_WRONLY | O_CREAT, 0666);
    FILE* file = fdopen (fd, "w");
    if (file == nullptr) {
        throw std::runtime_error ("fopen");
    }

    auto end = system_clock::now ();
    std::cout << "fopen: " << to_ms (begin, end) << " ms" << std::endl;

    begin = system_clock::now ();
    fwrite (str.data (), str.length (), 1, file);
    end = system_clock::now ();
    std::cout << "fwrite: " << to_ms (begin, end) << " ms" << std::endl;

    begin = system_clock::now ();
    fclose (file);
    end = system_clock::now ();
    std::cout << "fclose: " << to_ms (begin, end) << " ms" << std::endl;
}

// Получается, что fwrite сначала удаляет файл полностью, а write этого не делает,
// поэтому он гораздо быстрее
// Если файла нет: 0.42
// Если файл уже есть: 0.19
void
write_str_mega_c (const std::string& str,
                  const std::string& name)
{
    auto begin = system_clock::now ();
    int file_dtor = open64 (name.data (), O_RDWR | O_CREAT, 0666);
    if (file_dtor == -1) {
        throw std::runtime_error ("open");
    }

    auto end = system_clock::now ();
    std::cout << "open: " << to_ms (begin, end) << std::endl;

    begin = system_clock::now ();
    if (write (file_dtor, str.data (), str.size ()) != str.size ()) {
        throw std::runtime_error ("write");
    }
    end = system_clock::now ();
    std::cout << "write: " << to_ms (begin, end) << " ms" << std::endl;

    char* read_buf = new char[str.size ()];
    lseek64 (file_dtor, 0, SEEK_SET);

    begin = system_clock::now ();
    if (auto len = read (file_dtor, read_buf, str.size ()); len != str.size ()) {
        std::cerr << "len: " << len << std::endl;
        throw std::runtime_error ("read");
    }
    end = system_clock::now ();
    std::cout << "read: " << to_ms (begin, end) << " ms" << std::endl;

    std::string_view str_read (read_buf, str.size ());
    if (str_read != str) {
        std::cout << "error" << std::endl;
    }

    delete read_buf;

    begin = system_clock::now ();
    close (file_dtor);
    end = system_clock::now ();
    std::cout << "close: " << to_ms (begin, end) << " ms" << std::endl;
}

int main () try {
    // seclib::RandomGenerator rand;

    const std::size_t kb =         1'000;
    const std::size_t mb =     1'000'000;
    const std::size_t gb = 1'000'000'000;

    const std::size_t len = 2 * gb;
    const char min = 'a', max = 'd';
    const std::string output_file = "res.txt";

    std::cout << "len: " << len / kb << " kbytes" << std::endl;

    // std::string str = rand.get_string (len, min, max);
    std::string str;
    str.reserve (len);
    for (std::size_t i = 0; i < len; ++i) {
        str += min + i % (max - min);
    }

    auto begin = std::chrono::system_clock::now ();

#if 1
    std::cout << "write_str_mega_c" << std::endl;
    write_str_mega_c (str, output_file); // open, write
#else
    std::cout << "write_str_stream_fbuf" << std::endl;
    write_str_stream_fbuf (str, output_file); // fopen, fwrite 0.41
#endif

    auto end = std::chrono::system_clock::now ();

    std::cout << (end - begin).count () / 1000000000.0 << std::endl;
} catch (std::exception& exc) {
    std::cerr << "Error: " << exc.what () << std::endl;
}