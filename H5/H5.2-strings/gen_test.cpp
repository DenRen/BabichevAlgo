#include "solve.hpp"
#include "../../libs/other_func.hpp"
#include <fstream>

void
gen_test (std::string in_name, std::string out_name) {
    std::fstream is {in_name,  std::ios_base::out};
    std::fstream os {out_name, std::ios_base::out};

    seclib::RandomGenerator rand;

    auto str = rand.get_string (1000, 'a', 'b');
    auto res = native_solve (str);

    is << str;
    os << res << '\n';
}

int main () {
    gen_test ("test.txt", "test_ref.txt");
}