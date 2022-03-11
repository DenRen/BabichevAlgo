#include <fstream>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

void
gen_test (std::string in_name, std::string out_name) {
    const auto N = 500'000;
    const auto M = 500'000;

    std::fstream in {in_name, std::ios_base::out};
    std::fstream out {out_name, std::ios_base::out};

    in << N << " " << M << "\n";

    seclib::RandomGenerator rand;
    const auto vec = rand.get_vector <int> (N);
    for (const auto& elem : vec) {
        in << elem << "\n";
    }

    xor_array_native arr_ref {vec};

    for (std::size_t i = 0; i < M; ++i) {
        bool req_type = rand.get_rand_val <bool> ();
        if (req_type) {
            const auto l = rand.get_rand_val <unsigned> (N);
            const auto r = rand.get_rand_val <unsigned> (l, N);

            in << "1 " << l << " " << r << "\n";

            out << arr_ref.xor_range (l, r) << "\n";
        } else {
            const auto pos = rand.get_rand_val <std::size_t> (N);
            const auto val = rand.get_rand_val <unsigned> ();

            in << "2 " << pos << " " << val << "\n";

            arr_ref.update (pos, val);
        }
    }
}

int main () {
    gen_test ("test.txt", "test_ref.txt");
}