#include <iostream>

bool
is_pow_2 (std::size_t n ) {
    return (n & (n - 1)) == 0;
}

std::size_t
msb (std::size_t n) {
    int res = 0;
    while (n) {
        n >>= 1;
        ++res;
    }

    return res;
}

int main () {
    // 1 <= 100'000 -> (1 << 17) > 100'000

    std::size_t n = 0, k = 0;
    std::cin >> n >> k;
    
    if (k > 17) {
        k = 17;
    }

    std::size_t step = 1 << k;
    if (n <= step) {
        std::cout << msb (n) - is_pow_2 (n) << '\n';    
    } else {
        step = k == 1 ? 0 : 1 << (k - 1);
        std::cout << (n - 1) / (1 + step) + k - 1 << '\n';
    }
}
