#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <malloc.h>
#include <sys/stat.h>

int main () {
    const std::size_t kb =         1'000;
    const std::size_t mb =     1'000'000;
    const std::size_t gb = 1'000'000'000;

    const std::size_t len = 1 * gb;

    int fd = open ("res.txt", O_RDONLY);
    if (fd == -1) {
        throw std::runtime_error ("open");
    }

    struct stat file_stat = {0};
    if (fstat (fd, &file_stat) == -1) {
        throw std::runtime_error ("fstat");
    }

    std::cout << "block size: " << file_stat.st_blksize << "\n";

    // auto* str_buf = (char*) aligned_alloc (4096*2, len);
    auto* str_buf = (char*) aligned_alloc (file_stat.st_blksize, len * sizeof (char));
    if (str_buf == nullptr) {
        throw std::runtime_error ("malloc");
    }

    if (read (fd, str_buf, len) != len) {
        throw std::runtime_error ("read");
    }

    std::cout << *(char*) &str_buf[3] << std::endl;

    free (str_buf);
    close (fd);
}