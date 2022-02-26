// Challange: сдать с первого раза
#include <string>
#include <iostream>
#include "solve.hpp"
#include "debug_func.h"

int main () {
    const unsigned max_ram_size = 256 * 1024*1 + 60*0;    // 256 Кбайт
    const unsigned max_str_len = 10'000*1 + 28*0;           // 10000 байт
    std::string input_file_name  = "input.txt";
    std::string output_file_name = "output.txt";

    std::cout << "[" << max_ram_size << ", " << max_str_len << "]\n";

    solve (max_ram_size, max_str_len,
           input_file_name, output_file_name);

    return 0;
}