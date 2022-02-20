// Challange: сдать с первого раза
#include <string>
#include "solve.hpp"
#include "debug_func.h"

int main () {
    const unsigned max_ram_size = 256 * 1024*0 + 50;       // 256 Кбайт
    const unsigned max_str_len = 10'000*0 + 10;            // 10000 байт
    std::string input_file_name  = "input.txt";
    std::string output_file_name = "output.txt";

    CHECK_ERR (sovle (max_ram_size, max_str_len,
                      input_file_name, output_file_name));

    return 0;
}