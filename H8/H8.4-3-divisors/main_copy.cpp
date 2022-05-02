#include "../numerus/numerus.hpp"

std::ostream&
solve (std::ostream& os,
       unsigned n) {
    auto primes = nrs::calc_primes <unsigned> (n);
    std::vector <unsigned> nums (n + 1);

    for (unsigned i = 2; i < primes.size (); ++i) {
        auto p0 = primes[i];
        for (auto p = p0; p < nums.size (); p += p0) {
            ++nums[p];
        }
    }

    for (unsigned i = 2; i < nums.size (); ++i) {
        if (nums[i] >= 3) {
            os << i << ' ';
        }
    }

    return os;
}

int main () {
    std::ios_base::sync_with_stdio (false);

    unsigned n = 0;
    std::cin >> n;
    
    solve (std::cout, n) << '\n';
}
