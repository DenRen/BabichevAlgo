#include <iostream>

bool
is_pow_2 (std::size_t n ) {
    return (n & (n - 1)) == 0;
}

std::size_t
msb_up (std::size_t n) {
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
    } else if (k == 1) {
        std::cout << n - 1 << '\n';
        return 0;
    } else if (k == 0) {
        std::cout << -1 << '\n';
        return 0;
    }

    std::size_t step = 1 << k;
    if (n <= step) {
        std::cout << msb_up (n) - is_pow_2 (n) << '\n';    
    } else {
        step >>= 1;
        std::cout << (n / step) - 1 + k - is_pow_2 (n);
    }

    return 0;
}
