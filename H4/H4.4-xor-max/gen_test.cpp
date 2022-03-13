#include "solve.hpp"
#include "../../libs/other_func.hpp"
#include <fstream>
void
gen_test (std::string in_name, std::string out_name) {
    std::fstream is {in_name,  std::ios_base::out};
    std::fstream os {out_name, std::ios_base::out};

    seclib::RandomGenerator rand;

    const auto N = 500'000;

    const unsigned value_min = 0;
    const unsigned value_max = -1;

    is << 2 * N + 2 * (N / 10) << "\n";

    xor_array_map arr;
    // Insert
    for (unsigned i = 0; i < N; ++i) {
        auto value = rand.get_rand_val <unsigned> (value_min, value_max);
        arr.insert (value);
        is << "1 " << value << "\n";
    }

    for (unsigned i = 0; i < N; ++i) {
        auto value = rand.get_rand_val <unsigned> (value_min, value_max);
        is << "3 " << value << "\n";
        os << arr.max_xor (value) << "\n";
    }

    // Remove
    for (unsigned i = 0; i < 20; ++i) {
        auto value = rand.get_rand_val <unsigned> (value_min, value_max);
        arr.remove (value);
        is << "2 " << value << "\n";

    }
    
    for (unsigned i = 0; i < N / 10; ++i) {
        auto value = rand.get_rand_val <unsigned> (value_min, value_max);
        is << "3 " << value << "\n";
        os << arr.max_xor (value) << "\n";
    }
}

int main () {
    gen_test ("test.txt", "test_ref.txt");
}