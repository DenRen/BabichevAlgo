#include "solve.hpp"
#include "../../libs/other_func.hpp"
#include <fstream>

void
gen_test (std::string in_name, std::string out_name) {
    std::fstream is {in_name,  std::ios_base::out};
    std::fstream os {out_name, std::ios_base::out};

    int N = 2000;
    is << N << '\n';

    seclib::RandomGenerator rand;
    for (int i = 0; i < N; ++i) {
        auto str = rand.get_string (N, '0', '1');
        is << str << '\n';
    }
}

int main () {
    gen_test ("test.txt", "test_ref.txt");
}
