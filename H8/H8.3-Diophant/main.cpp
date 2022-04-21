#include <iostream>
#include <vector>
#include <cmath>

#include "../../libs/other_func.hpp"

int
msb (std::size_t n) {
    int res = 0;
    while (n >>= 1) {
        ++res;
    }

    return res;
}

std::size_t
fast_pow (std::size_t x, std::size_t pow, std::size_t m) {
    std::size_t res = 1;
    
    int num_bits = msb (pow) + 1;
    std::size_t bit_mask = pow << (64 - num_bits);

    while (num_bits--) {
        res *= res;
        res %= m;
        if (bit_mask & (1ull << 63)) {
            res *= x;
            res %= m;
        }

        bit_mask <<= 1;
    }

    return res;
}

std::size_t
solve (std::size_t X, std::size_t P, std::size_t M) {
    return fast_pow (X % M, P, M);
}

std::vector <bool>
build_prim_sieve (std::size_t n) {
    // Atkin's sieve
    std::vector <bool> b (n, false);
    b[2] = b[3] = true;

    std::size_t root_n = sqrt (n);

    for (std::size_t x = 1; x < root_n; ++x) {
        const auto x_sqrt = x * x;
        for (std::size_t y = 1; y < root_n; ++y) {
            const auto y_sqrt = y * y;

            auto t1 = 4 * x_sqrt + y_sqrt;
            auto t2 = 3 * x_sqrt + y_sqrt;
            auto t3 = 3 * x_sqrt - y_sqrt;
            
            if (t1 < n && (t1 % 12 == 1 || t1 % 12 == 5)) b[t1] = !b[t1];
            if (t2 < n && (t2 % 12 == 7)) b[t2] = !b[t2];
            if (t3 < n && (t3 % 12 == 11) && x > y) b[t3] = !b[t3];
        }
    }

    for (std::size_t i = 2; i < root_n; ++i) {
        if (b[i]) {
            b[i * i] = false;
        }
    }
    
    return b;
}

long
gcd (long a, long b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }

    return a > b ? gcd (a - b, b) : gcd (b - a, a);
}

std::size_t
comprime_number (std::size_t n) {
    int res = 0;
    for (std::size_t i = 1; i < n; ++i) {
        res += gcd (n, i) == 1;
    }

    return res;
}

std::size_t
get_reciprical (std::size_t a, std::size_t m) {
    return fast_pow (a, comprime_number (m) - 1, m);
}

std::pair <long, long>
solve_ax_by_0 (long a, long b) {
    long x = std::abs (a * b) / gcd (std::abs (a), std::abs (b)); // lcm (a, b)
    long y = -a * x / b;
    
    return {x, y};
}

long
solve_ax_0_c (long a, long c, bool& is_no) {
    if (a * c > 0) {
        if (c % a == 0) {
            return c / a;
        }        
    }

    is_no = true;
    return -1;
}

long
solve_0_by_c (long b, long c, bool& is_no) {
    if (c % b == 0) {
        return c / b;
    }

    is_no = true;
    return -1;
}

std::pair <long, long>
solve_ax_by_c (long a, long b, long c, bool& is_no) {
    if (gcd (std::abs (a), std::abs (b)) % std::abs (c) != 0) {
        is_no = true;
        return {-1, -1};
    }

    if (b < 0) {
        a = -a;
        b = -b;
        c = -c;
    }
    
    long abs_a = a > 0 ? a : b + a % b;
    long abs_c = c > 0 ? c : b + c % b;
    long x = get_reciprical (abs_a, b) * abs_c % b;
    long y = (c - a * x) / b;

    is_no = a * x + b * y != c;
    return {x, y};
}

std::pair <bool, std::pair <long, long>>
solve_diaf (long a, long b, long c) {
    bool is_no = false;
    long x = 0, y = 0;
    
     // ax + by = c
    if (c == 0) {       // ax + by = O
        if (a == 0) {   // by = 0;
            x = 1;
            y = 0;
        } else if (b == 0) { // ax = 0
            is_no = true;
        } else {        // ax + by = 0, a != 0, b != 0
            auto res = solve_ax_by_0 (a, b);
            x = res.first;
            y = res.second;
        }
    } else {            // ax + by = C
        if (b == 0) {   // ax = c
            if (a == 0) {
                is_no = true;
            } else {
                x = solve_ax_0_c (a, c, is_no);
            }
        } else if (a == 0) { // By = C 
                x = 1;
                y = solve_0_by_c (b, c, is_no);
        } else {    
            auto res = solve_ax_by_c (a, b, c, is_no);
            x = res.first;
            y = res.second;
        }
    }

    return {is_no, {x, y}};
}

void
test () {
    seclib::RandomGenerator rand;

    auto vec_a = rand.get_vector <long> (100);
    auto vec_b = rand.get_vector <long> (100);
    auto vec_y = rand.get_vector <long> (100);
    long x = 1;

    for (const auto& a : vec_a)
    for (const auto& b : vec_b)
    for (const auto& y : vec_y) {
        if (a == 0 || b == 0) {
            continue;
        }

        auto c = a * x + b * y;
        
        auto[is_no, res] = solve_diaf (a, b, c);
        if (is_no) {
            std::cerr << "Error is_no == true!\n";
            return;
        }

        if (res.first != x || res.second != y) {
            std::cerr << "Error res:" << res.first << ' ' << res.second << '\n'
                      << "But ref: " << x << ' ' << y << '\n';
            return;
        }
    }
}

int main () {
    std::ios_base::sync_with_stdio (false);
        
    test ();
    return 0;

    long a = 0, b = 0,c = 0;
    std::cin >> a >> b >> c;
    
    auto [is_no, res] = solve_diaf (a, b, c);
    
    if (is_no) {
        std::cout << "No\n";
    } else {
        const auto& x = res.first, &y = res.second;
        std::cout << x << ' ' << y << '\n';
        //std::cout << ((a * x + b * y == c) ? "Good" : "Fuck") << '\n';
    }
}
