#include <fstream>

#include "../../libs/other_func.hpp"

std::vector <std::string>
gen_str_file (unsigned num_lines,
              unsigned max_str_line)
{
    seclib::RandomGenerator rand;

    std::vector <std::string> strs;
    strs.reserve (num_lines);

    while (num_lines--) {
        unsigned str_len = rand.get_rand_val <unsigned> (1, max_str_line);

        std::string str;
        str.reserve (str_len);
        while (str_len--) {
            str += rand.get_rand_val <unsigned char> ('a', 'z');
        }

        strs.push_back (std::move (str));
    }

    return strs;    
}

void
write_file (const std::vector <std::string>& strs,
            const std::string& file_name)
{
    std::ofstream os {file_name};
    for (const auto& str : strs) {
        os << str << std::endl;
    }
}

int main () {
    unsigned num_lines = 10'000'000 / 100;

    const unsigned max_ram_size = 256 * 1024;
    const unsigned max_str_len = 10'000;

    std::string output_file_name = "test_input.txt";
    std::string solve_file_name  = "test_input_sovle.txt";
    
    auto strs = gen_str_file (num_lines, max_str_len);
    write_file (strs, output_file_name);

    std::sort (strs.begin (), strs.end ());
    write_file (strs, solve_file_name);
}