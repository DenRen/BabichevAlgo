#include "solve.hpp"
#include "../../libs/other_func.hpp"
#include <fstream>

void
gen_test (std::string in_name, std::string out_name) {
    std::fstream is {in_name,  std::ios_base::out};
    std::fstream os {out_name, std::ios_base::out};

    seclib::RandomGenerator rand;
    auto str = rand.get_string (1000000, 'a', 'z');
    auto dstr = solve (str);

    is << str;
    os << dstr << "\n";
}

int main () {
    gen_test ("test.txt", "test_ref.txt");
}