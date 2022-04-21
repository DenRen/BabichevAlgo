#include <iostream>
#include <numeric>
#include <tuple>

int
gcd (int a, int b) {
    
    while (b != 0) {
        a %= b;
        std::swap (a, b);
    }

    return a ? a : b;
}

std::tuple <long, long, long>
ext_euclid (long a, long b) {
    long u1 = 1, v1 = 0, u2 = 0, v2 = 1;
    while (b != 0) {
        long q = a / b, r = a % b;

        long u1_ = u2, v1_ = v2;
        long u2_ = u1 - q * u2;
        long v2_ = v1 - q * v2;

        a = b;
        b = r;

        u1 = u1_;
        u2 = u2_;
        v1 = v1_;
        v2 = v2_;
    }

    return {a, u1, v1};
}

int main () {
    // -2 3 10
    auto[d, u, v] = ext_euclid (-2, 3);
    std::cout << d << ' ' << u << ' '  << v<< '\n';

    return 0;
    for (int i = 1; i < 10000; ++i)
        for (int j = 1; j < 10000; ++j)
            if (auto[d, u, v] = ext_euclid (i, j); i * u + j * v != d) {
                std::cout << "F\n";
                return 0;
            }
    return 0;
    while (true) {
        int a = 0, b = 0;
        std::cin >> a >> b;
        std::cout << gcd (a, b) << std::endl;
        std::cout << std::gcd (a, b) << std::endl;
    }
}
