#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include "../../../libs/other_func.hpp"


template <typename T>
double
to_ms (std::chrono::time_point <T> begin,
       std::chrono::time_point <T> end)
{
    return (end - begin).count () / 1000000.0;
}

using std::chrono::system_clock;

void
write_str_stream_it (const std::string& str,
                     const std::string& name)
{
    // Если файла нет: 0.42
    auto begin = system_clock::now ();
    std::ofstream os {name};
    auto end = system_clock::now ();
    std::cout << "ofstream ctor: " << to_ms (begin, end) << " ms" << std::endl;

    begin = system_clock::now ();
    os << str;
    end = system_clock::now ();
    std::cout << "os << str: " << to_ms (begin, end) << " ms" << std::endl;
}

// 0.41
void
write_str_c (const std::string& str,
             const std::string& name)
{
    auto begin = system_clock::now ();

    FILE* file = fopen (name.data (), "w");
    if (file == nullptr) {
        throw std::runtime_error ("fopen");
    }

    auto end = system_clock::now ();
    std::cout << "fopen: " << to_ms (begin, end) << " ms" << std::endl;

    begin = system_clock::now ();
    fwrite (str.data (), 1, str.length (), file);
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
    int file_dtor = open (name.data (), O_WRONLY | O_CREAT, 0666);
    if (file_dtor == -1) {
        throw std::runtime_error ("open");
    }

    auto end = system_clock::now ();
    std::cout << "open: " << to_ms (begin, end) << std::endl;

    begin = system_clock::now ();
    if (write (file_dtor, str.data (), str.length ()) != str.length ()) {
        throw std::runtime_error ("write");
    }
    end = system_clock::now ();
    std::cout << "write: " << to_ms (begin, end) << " ms" << std::endl;

    begin = system_clock::now ();
    close (file_dtor);
    end = system_clock::now ();
    std::cout << "close: " << to_ms (begin, end) << " ms" << std::endl;
}

int main () {
    seclib::RandomGenerator rand;

    const std::size_t len = 1'000'000'000;
    const char min = 'a', max = 'd';
    const std::string output_file = "res.txt";

    std::string str = rand.get_string (len, min, max);

    auto begin = std::chrono::system_clock::now ();

#if 0
    std::cout << "write_str_mega_c" << std::endl;
    write_str_mega_c (str, output_file); // open, write
#else
    std::cout << "write_str_stream_it" << std::endl;
    write_str_stream_it (str, output_file); // fopen, fwrite 0.41
#endif

    auto end = std::chrono::system_clock::now ();

    std::cout << (end - begin).count () / 1000000000.0 << std::endl;
}