#include "solve.hpp"
#include "../../libs/other_func.hpp"
#include <fstream>

void
test_median_random (std::string in_name, std::string out_name) {
    const auto M = 500'000 / 2;

    std::fstream in  {in_name,  std::ios_base::out};
    std::fstream out {out_name, std::ios_base::out};

    in << M << "\n";

    seclib::RandomGenerator rand;
    median_array_native <int> arr_ref;

    {
        auto first_value = rand.get_rand_val <int> ();
        arr_ref.add (first_value);
        in << "0 " << first_value << "\n";
    }

    for (std::size_t i = 0; i < M - 1; ++i) {
        bool req_type = rand.get_rand_val <unsigned> () % 2;
        if (req_type) {
            const auto val = rand.get_rand_val <int> ();

            arr_ref.add (val);
            in << "0 " << val << "\n";
        } else {
            in << "1\n";
            out << arr_ref.median () << "\n";
        }
    }
}

int main () {
    test_median_random ("test.txt", "test_ref.txt");
}