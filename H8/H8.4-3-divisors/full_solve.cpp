#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
#include <complex>

// g++ -DHOST -std=c++17 main.cpp

#define NDEBUG
// #define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cerr << #obj ": " << obj << '\n'
#else
    #define DUMP(obj)
#endif

namespace nrs {

// Bit funcs
template <typename T>
unsigned
msb (T n) {
    unsigned res = 0;
    while (n >>= 1ull) {
        ++res;
    }

    return res;
}

template <typename T>
T
is_pow_2_positive (T n) {
    return (n & (n - 1)) == 0;
}

template <typename T>
T
is_pow_2 (T n) {
    return (n > 0) && is_pow_2_positive (n);
}

template <typename T>
T
increase_pow_2 (T n) {
    return is_pow_2 (n) ? n : static_cast <T> (1) << (msb (n) + (n > 0));
}

// Primes generators
std::vector <bool>
calc_sieve (std::size_t n) {
    std::vector <bool> sieve (n + 1, true);

    for (std::size_t p = 2; p <= n; ++p) {
        if (sieve[p]) {
            for (std::size_t i = p * p; i <= n; i += p) {
                sieve[i] = false;
            }
        }
    }

    return sieve;
}

template <typename T>
std::vector <T>
sieve2vec (const std::vector <bool>& sieve) {
    std::vector <T> vec;
    vec.reserve (1 + std::sqrt (sieve.size ()));

    for (std::size_t i = 0; i < sieve.size (); ++i) {
        if (sieve[i]) {
            vec.push_back (i);
        }
    }

    return vec;
}

template <typename T>
std::vector <T>
calc_primes (std::size_t N) {
    auto sieve = calc_sieve (N);
    auto primes = sieve2vec <T> (sieve);
    return primes;
}


template <typename T>
T
fast_pow (T x, std::size_t pow, T m) {
    T res = 1;

    auto num_bits = msb (pow) + 1;
    T bit_mask = static_cast <T> (pow) << (8 * sizeof (T) - num_bits);

    while (num_bits--) {
        res = res * res % m;

        T msb_pos = static_cast <T> (1) << (8 * sizeof (T) - 1);
        if (bit_mask & msb_pos) {
            res = res * x % m;
        }

        bit_mask <<= 1;
    }

    return res;
}

template <typename T>
bool
is_prime_native (T n) { // n >= 0
    if (n <= 3) {
        return true;
    }

    if (n % 2 == 0) {
        return false;
    }

    auto root_n = std::sqrt (n);
    for (T i = 3; i <= root_n; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

// n is odd => n > 0
template <typename T>
bool
is_strong_pseudoprime_impl (T n, unsigned base) {
    // n = d * 2 ^ s + 1
    assert (n > 0);

    if (n == 1) {
        return true;
    }

    --n;

    // calc s
    unsigned s = 1;
    auto d = n / 2;
    while (d % 2 == 0) {
        d /= 2;
        ++s;
    }
    ++n;

    // First check a^d = 1 mod n
    base %= n;
    auto a_pow_d = fast_pow <T> (base, d, n);
    if (a_pow_d == 1 || a_pow_d == n - 1) {
        return true;
    }

    // Second check a ^ (a * 2 ^ r) = -1 mod n, 0 <= r < s
    for (unsigned r = 1; r < s; ++r) {
        a_pow_d *= a_pow_d;
        a_pow_d %= n;
        if (a_pow_d == n - 1) {
            return true;
        }
    }

    return false;
}

template <typename T>
bool
is_strong_pseudoprime (T n, unsigned base) {
    constexpr unsigned bits = 4 * sizeof (T);
    constexpr std::size_t root_max = static_cast <T> (1) << bits;
    const bool error_base = base > root_max;
    const bool error_n = n > root_max;

    if (error_base || error_n) { // Good
        return is_strong_pseudoprime_impl <__uint128_t> (n, base);
    } else { // Overflow with T, must use U: sizeof (U) >= 2 * sizeof (T)
        return is_strong_pseudoprime_impl <uint64_t> (n, base);
    }
}

std::size_t
comprime_number (std::size_t n) {
    std::size_t res = 0;
    for (std::size_t i = 1; i < n; ++i) {
        res += std::gcd (n, i) == 1;
    }

    return res;
}

std::size_t
get_reciprical (std::size_t a, std::size_t m) {
    return fast_pow (a, comprime_number (m) - 1, m);
}

// m is prime
std::size_t
get_reciprical_prime (std::size_t a, std::size_t m) {
    return fast_pow (a, m - 2, m);
}

class is_prime_tester {
    const std::size_t num_primes = 1 << 16;
    std::vector <bool> sieve;

public:
    is_prime_tester () :
        sieve (calc_sieve (num_primes))
    {}

    bool
    operator () (std::size_t N) const {
        return is_prime (N);
    }

    template <typename T>
    bool
    is_prime (T n) const {
        if (n <= num_primes && sieve[n]) {
            return true;
        }

        if (n % 2 == 0) {
            return false;
        }

        /*
        0) n <                             2'047 простое, если оно сильно псевдопростое по основаниям a = 2;
        1) n <                         1'373'653 a = 2, 3;
        2) n <                        25'326'001 a = 2, 3, 5;
        3) n <                     3'215'031'751 a = 2, 3, 5, 7;
        4) n <                 2'152'302'898'747 a = 2, 3, 5, 7, 11;
        5) n <                 3'474'749'660'383 a = 2, 3, 5, 7, 11, 13;
        6) n <               341'550'071'728'321 a = 2, 3, 5, 7, 11, 13, 17;
        7) n <         3'825'123'056'546'413'051 a = 2, 3, 5, 7, 11, 13, 17, 19, 23;
        8) n <   318'665'857'834'031'151'167'461 a = 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37;
        9) n < 3'317'044'064'679'887'385'961'981 a = 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41;

        2 ^ 64 =      18'446'744'073'709'551'615 => max is 8
        */

        unsigned type = 0;
        type += n >=          2047;
        type += n >=     1'373'653;
        type += n >=    25'326'001;
        type += n >= 3'215'031'751;

        if constexpr (sizeof (T) > 4) {
            type += n >=         2'152'302'898'747;
            type += n >=         3'474'749'660'383;
            type += n >=       341'550'071'728'321;
            type += 2 * (n >= 3'825'123'056'546'413'051);
        }

        const unsigned bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
        for (unsigned i = 0; i <= type; ++i) {
            if (is_strong_pseudoprime (n, bases[i]) == false) {
                return false;
            }
        }

        return true;
    }

    const std::vector <bool>
    get_sieve () const noexcept {
        return sieve;
    }
};

template <typename T, typename Rand>
T
ro_pollard (T n,
            Rand& rand) {
    T x = static_cast <T> (1) + rand () % (n - 2);
    T y = 1, i = 0, stage = 2;

    while (std::gcd (n, x - y) == 1) {
        if (i == stage) {
            y = x;
            stage *= 2;
        }

        x = (x * x + 1) % n;    // +

        ++i;
    }

    return std::gcd (n, x - y);
}

class factorizer {
    is_prime_tester is_prime;
    std::vector <unsigned> primes;
    mutable std::mt19937_64 rand;

    template <typename T>
    void
    put_mults (T n,
               std::vector <T>& mults) const {
        // std::cout << "put_mults: " << n << std::endl;
        while (n != 1) {
            if (is_prime (n)) {
                mults.push_back (n);
                return;
            }

            T mult = ro_pollard (n, rand);
            while (mult == n) {
                 mult = ro_pollard (n, rand);
            }

            if (is_prime (mult)) {
                mults.push_back (mult);
            } else {
                put_mults (mult, mults);
            }

            n /= mult;
        };
    }

    template <typename T>
    void
    put_mults2 (T n,
                std::vector <T>& mults) const {
        while (n != 1) {
            if (is_prime (n)) {
                mults.push_back (n);
                return;
            }

            T mult = p_minus_1_pollard (n);
            while (mult == n) {
                mult = p_minus_1_pollard (n);
            }

            if (is_prime (mult)) {
                mults.push_back (mult);
            } else {
                put_mults2 (mult, mults);
            }

            n /= mult;
        };
    }

    template <typename T>
    T
    p_minus_1_pollard (T N) const {
        // Stage 1
        T B = 1 + rand () % 27;
        T M = 1, q = 0, b = 0;

        do {
            for (std::size_t i = 2; i < primes.size (); ++i) {
                auto p = primes[i];
                if ((long)p >= B) {
                    break;
                }

                for (T b1 = B; b1 /= p; M *= p)
                    ;
            }

            T a = 1 + rand ();
            b = fast_pow <__uint128_t> (a % N, M, N);
            q = std::gcd (b - 1, N);
        } while (q == N);

        if (q != 1 && q != N) {
            return q;
        }

        // Stage 2
        T B1 = B, B2 = B * B;
        std::vector <T> qs;
        for (T l = B1 + (B1 % 2 == 0); l <= B2; l += 2) {
            if (is_prime (l)) {
                qs.push_back (l);
            }
        }

        T max_D = 0;
        std::vector <T> D (qs.size ());
        for (std::size_t i = 0; i + 1 < qs.size (); ++i) {
            D[i] = qs[i + 1] - qs[i];
            if (D[i] > max_D) {
                max_D = D[i];
            }
        }

        std::vector <__uint128_t> b_d (max_D + 1);
        b_d[0] = 1;
        for (std::size_t i = 1; i < b_d.size (); ++i) {
            b_d[i] = b * b_d[i - 1] % N;
        }

        T c = 1;
        for (const auto& d : D) {
            c = c * b_d[d] % N;
            T G = std::gcd (c - 1, N);
            if (G != 1) {
                return G;
            }
        }

        return N;
    }

public:
    factorizer () :
        primes (sieve2vec <unsigned> (is_prime.get_sieve ())),
        rand (std::random_device {} ())
    {}

    template <typename T>
    auto
    operator () (T n) const {
        return to_factorize2 (n);
    }

    template <typename T>
    std::vector <T>
    to_factorize (T n) const {
        if (n <= 3) {
            return {n};
        }

        std::vector <T> mults;

        // Remove 2^q
        while (n % 2 == 0) {
            mults.push_back (2);
            n /= 2;
        }

        for (std::size_t i = 3; i < primes.size (); ++i) {
            auto p = primes[i];
            while (n % p == 0) {
                mults.push_back (p);
                n /= p;
            }
        }

        put_mults (n, mults);
        std::sort (mults.begin (), mults.end ());

        return mults;
    }

    template <typename T>
    std::vector <T>
    to_factorize2 (T n) const {
        if (n <= 3) {
            return {n};
        }

        std::vector <T> mults;

        // Remove 2^q
        while (n % 2 == 0) {
            mults.push_back (2);
            n /= 2;
        }

        for (std::size_t i = 3; i < primes.size (); ++i) {
            auto p = primes[i];
            while (n % p == 0) {
                mults.push_back (p);
                n /= p;
            }
        }

        put_mults2 (n, mults);
        std::sort (mults.begin (), mults.end ());

        return mults;
    }

private:
};

std::size_t
num_comb (std::size_t N, std::size_t K, std::size_t P) {
    if (P <= N) {
        return 0;
    }

    std::size_t L = N - K < K ? K + 1 : N - K + 1;
    std::size_t D = N - K < K ? N - K : K;

    std::size_t C = 1;
    for (std::size_t k = L; k <= N; ++k) {
        C *= k;
        C %= P;

        if (C == 0) {
            return 0;
        }
    }
    C %= P;

    std::size_t denom = 1;
    for (std::size_t l = 2; l <= D; ++l) {
        denom *= l;
        denom %= P;

        if (denom == 0) {
            return 0;
        }
    }
    denom %= P;

    std::size_t rec = fast_pow <std::size_t> (denom, P - 2, P);

    return C * rec % P;
}

// 1000000007
std::size_t
sum_num_comb_native (std::size_t n,
                     std::size_t l,
                     std::size_t r,
                     std::size_t m) {
    std::size_t res = 0;
    const auto k_max = std::min (n, l * r);
    for (std::size_t k = l; k <= k_max; k += l) {
        res = (res + num_comb (n, k, m)) % m;
    }

    return res;
}

namespace detail {

// r > 1
std::size_t
sum_num_comb_calc_k0 (std::size_t n, std::size_t l, std::size_t r) {
    // Check on just L
    if (l >= n - l) {
        return 1;
    }

    // Check on just R
    if (l * r < n - l * r) {
        return r + 1;
    }

    // Here only [R, L]
    for (std::size_t k = 2 * l; k <= r * l; k += l) {
        if (k >= n - k) {
            return k / l;
        }
    }

    // Here never!
    return -1;
}

} // namespace detail

// 1000000007
std::size_t
sum_num_comb (std::size_t n,
              std::size_t l,
              std::size_t r,
              std::size_t m) {
    if (m <= n) {
        return 0;
    }

    if (l > n) {
        return 0;
    }
    if (l * r > n) {
        r = n / l;
    }

    if (r == 1) {
        return num_comb (n, l, m);
    }

    std::size_t k0 = detail::sum_num_comb_calc_k0 (n, l, r);
    assert (k0 < -2ull);

    std::size_t R_size = k0 <= 1 ? 0 : k0 - 1;
    std::size_t L_size = r - R_size;

    std::size_t f_max = R_size != 0 && L_size != 0 ? std::max (R_size * l, n - k0 * l) :
                        R_size != 0 ? R_size * l : n - k0 * l;
    std::size_t f_min = R_size != 0 && L_size != 0 ? std::min (l, n - r * l) :
                        R_size != 0 ? l : n - r * l;
    std::size_t theta = R_size != 0 && L_size != 0 ? std::min (n - R_size * l, k0 * l) :
                        R_size != 0 ? n - R_size * l : k0 * l;

    std::vector <std::size_t> fs (f_max - f_min + 1);
    fs[0] = 1;
    for (std::size_t f = 2; f <= f_min; ++f) {
        fs[0] = fs[0] * f % m;
    }
    for (std::size_t f = f_min + 1; f <= f_max; ++f) {
        std::size_t i = f - f_min;
        fs[i] = fs[i - 1] * f % m;
    }

    std::vector <std::size_t> sigmas (n - theta + 1);
    sigmas[0] = 1;
    sigmas[1] = n;
    for (std::size_t i = 2; i < sigmas.size (); ++i) {
        sigmas[i] = (sigmas[1] - i + 1) * sigmas[i-1] % m;
    }

    std::size_t res = 0;

    if (msb (m) < l) {
        for (std::size_t k = 1; k <= R_size; ++k) {
            auto i = k * l;
            auto sigma = sigmas[i];
            std::size_t f = fs[i - f_min];
            if (sigma == 0 || f == 0) {
                break;
            }
            res += (sigma * get_reciprical_prime (f, m)) % m;
        }

        for (std::size_t k = 0; k + 1 <= L_size; ++k) {
            auto i = n + (k - r) * l;
            auto sigma = sigmas[i];
            std::size_t f = fs[i - f_min];
            if (sigma == 0 || f == 0) {
                break;
            }
            res += (sigma * get_reciprical_prime (f, m)) % m;
        }
    } else {
        if (R_size >= 1) {
            auto i_max = R_size * l;
            std::size_t f_rec = get_reciprical_prime (fs[i_max - f_min], m);
            res += (sigmas[i_max] * f_rec) % m;

            for (std::size_t k = R_size - 1; k >= 1; --k) {
                auto i = k * l;

                auto sigma = sigmas[i];
                if (sigma == 0) {
                    break;
                }

                for (std::size_t j = 1; j <= l; ++j) {
                    f_rec *= i + j;
                    f_rec %= m;
                }

                res += (sigma * f_rec) % m;
            }
        }

        if (L_size >= 1) {
            auto i_max = n + (L_size - 1 - r) * l;
            std::size_t f_rec = get_reciprical_prime (fs[i_max - f_min], m);
            res += sigmas[i_max] * f_rec % m;

            if (L_size >= 2) {
                for (std::size_t k = L_size - 2; k + 1ull > 0; --k) {
                    auto i = n + (k - r) * l;

                    auto sigma = sigmas[i];
                    if (sigma == 0) {
                        break;
                    }

                    for (std::size_t j = 1; j <= l; ++j) {
                        f_rec *= i + j;
                        f_rec %= m;
                    }

                    res += sigma * f_rec % m;
                }
            }
        }
    }

    return res % m;
}


namespace fft {

template <typename T>
std::vector <T>
mult_poli_native (const std::vector <T>& P,
                  const std::vector <T>& Q) {
    const auto P_size = P.size (), Q_size = Q.size ();

    std::vector <T> R (P.size () + Q.size () - 1, 0);
    for (std::size_t i = 0; i < P_size; ++i) {
        for (std::size_t j = 0; j < Q_size; ++j) {
            R[i + j] += P[i] * Q[j];
        }
    }

    return R;
}

template <typename T>
std::vector <T>
calc_rev_arr (std::size_t k) {
    const auto n = 1ull << k;
    std::vector <T> revs (n, 0);

    int oldest = -1;
    for (T mask = 1; mask < n; ++mask) {
        if (is_pow_2_positive (mask)) {
            ++oldest;
        }
        revs[mask] = revs[mask ^ (1ull << oldest)] | (1ull << (k - oldest - 1));
    }

    return revs;
}

template <typename T>
void
conv2first_line (std::vector <T>& vec, unsigned k) {
    std::vector <T> tmp;
    tmp.resize (vec.size ());

    auto revs = calc_rev_arr <unsigned> (k);
    for (unsigned i = 0; i < vec.size (); ++i) {
        tmp[i] = vec[revs[i]];
    }
    std::swap (tmp, vec);
}

// A.size () is pow 2
template <int mult, typename T>
void
mult_W (std::vector <std::complex <T>>& A) {
    std::size_t n = A.size ();
    unsigned k = msb (n);

    // Prepare first line
    conv2first_line (A, k);

    for (std::size_t j = 0; j < k; ++j) {
        // Prepare omegas
        std::vector <std::complex <T>> ws (2 + (1ull << j));
        double alpha = mult * M_PI / (1ull << j);
        ws[0] = 1;
        ws[1] = {std::cos (alpha), std::sin (alpha)};
        for (std::size_t k = 2; k < ws.size (); ++k) {
            ws[k] = ws[1] * ws[k-1];
        }
        // DUMP (ws);

        for (std::size_t i = 0; i < n; i += 1ull << (j + 1)) {
            for (std::size_t s = 0; s < (1ull << j); ++s) {
                auto x = A[i + s];
                auto y = A[i + s + (1ull << j)] * ws[s];
                A[i + s] = x + y;
                A[i + s + (1ull << j)] = x - y;
            }
        }
    }
}

template <typename T>
std::vector <double>
mult_poli (std::vector <T>& P,
           std::vector <T>& Q) {
    auto n0 = P.size () + Q.size () - 1;
    auto n = increase_pow_2 (n0);

    std::vector <std::complex <double>> Pc (n, 0), Qc (n, 0);
    std::copy (P.cbegin (), P.cend (), Pc.begin ());
    std::copy (Q.cbegin (), Q.cend (), Qc.begin ());

    mult_W <+1> (Pc);
    mult_W <+1> (Qc);

    std::vector <std::complex <double>> Rc (n, 0);
    for (std::size_t i = 0; i < n; ++i) {
        Rc[i] = Pc[i] * Qc[i];
    }

    mult_W <-1> (Rc);

    std::vector <double> R (n);
    for (std::size_t i = 0; i < n; ++i) {
        // R[i] = std::round (Rc[i].real ()) / n;
        R[i] = Rc[i].real () / n;
    }

    R.resize (n0);
    return R;
}

} // namespace fft

} // namespace nrs

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
