#include "solve.hpp"
#include "../../libs/other_func.hpp"
#include <chrono>

int
msd_native (uint64_t n) {
    int r = 0;
    while (n >>= 1) {
        ++r;
    }

    return r;
}

int
msd (uint64_t n) {
    unsigned r = 0, k = 32;
    while (n > 1) {
        if (n >> k) {
            n >>= k;
            r += k;
        }
        k /= 2;
    }

    return r;
}

int main () {
    seclib::RandomGenerator rand;
    
    std::size_t N = 100000;
    int repeat = 1000;

    auto nums = rand.get_vector <uint64_t> (N);
    std::vector <uint64_t> res_n (N), res (N);

    std::cout << "Nums generated\n";

    using time = std::chrono::high_resolution_clock;

    auto t_n_b = time::now ();
    for (int _r = 0; _r < repeat; ++_r) {
        for (std::size_t i = 0; i < N; ++i) {
            res_n[i] = msd_native (nums[i]);
        }
    }
    auto t_n_e = time::now ();

    auto t_b = time::now ();
    for (int _r = 0; _r < repeat; ++_r) {
        for (std::size_t i = 0; i < N; ++i) {
            res[i] = msd (nums[i]);
        }
    }
    auto t_e = time::now ();

    for (std::size_t i = 0; i < N; ++i) {
        if (res_n[i] != res[i]) {
            std::cerr << i << " -> Failed\n";
            return 0;
        }
    }

    auto dt_n = t_n_e - t_n_b, dt = t_e - t_b;

    std::cout << "native: " << dt_n.count () / 1000000 << '\n';
    std::cout << "myself: " << dt.count () / 1000000 << '\n';

    return 0;
}
