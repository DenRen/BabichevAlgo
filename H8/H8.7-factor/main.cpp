#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

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

std::vector <std::size_t>
sieve2vec (const std::vector <bool>& sieve) {
    std::vector <std::size_t> vec;
    vec.reserve (sieve.size ());

    for (std::size_t i = 0; i < sieve.size (); ++i) {
        if (sieve[i]) {    
            vec.push_back (i);
        }
    }

    return vec;
}

template <typename T>
std::ostream&
operator << (std::ostream& os,
             const std::vector <T>& vec)
{
    const std::size_t size = vec.size ();
    if (size == 0) {
        return os;
    }

    for (std::size_t i = 0; i + 1 < size; ++i) {
        os << vec[i] << " ";
    }

    return os << vec[size - 1];
}

template <typename T>
T
F (T x, std::size_t N) {
    return (x * x - 1) % N;
}

template <typename Rand>
std::size_t
ro_pollard (std::size_t n,
            Rand& rand) {
    std::size_t x = rand () % 10ull;
    std::size_t y = 1, i = 0, stage = 2;

    while (std::gcd (n, x - y) == 1) {
        if (i == stage) {
            y = x;
            stage *= 2;
        }

        x = (x * x + 1) % n;
        ++i;
    }

    return std::gcd (n, x - y);
}

template <typename Rand>
void
put_mults (std::size_t n,
           std::vector <std::size_t>& mults,
           Rand& rand,
           const std::vector <bool>& is_prime) {
    while (n != 1) {
        if ((n & 1) == 0) {
            mults.push_back (2);
            n /= 2;
            continue;
        }

        std::size_t mult = ro_pollard (n, rand); 
        if (is_prime [mult]) {    // n - prime
            mults.push_back (mult);
            n /= mult;
            continue;
        }
        
        put_mults (mult, mults, rand, is_prime);
        n /= mult;
    };
}

std::vector <std::size_t>
solve (std::size_t n) {
    std::vector <std::size_t> mults;
    
    std::random_device rd {};
    std::mt19937 rand (rd ());

    auto sieve = calc_sieve (n);
    put_mults (n, mults, rand, sieve);
    std::sort (mults.begin (), mults.end ());

    return mults;
}

std::vector <std::size_t>
solve2 (std::size_t n) {
    std::vector <std::size_t> mults;
    
    std::size_t root_n = std::sqrt (n);
    for (std::size_t i = 2; i <= root_n; ++i) {
        while (n % i == 0) {
            mults.push_back (i);
            n /= i;
            root_n = sqrt (n);
        }
    }

    if (n != 1) {
        mults.push_back (n);
    }

    std::sort (mults.begin (), mults.end ());
    return mults;
}

void
test (std::size_t N) {
    auto ps = sieve2vec (calc_sieve (N));
    auto n = ps.size ();

    for (std::size_t x = 2; x < n; ++x) {
    for (std::size_t y = 2; y < n; ++y) {
    for (std::size_t z = 2; z < n; ++z) {
    for (std::size_t w = 2; w < n; ++w) {
        std::size_t num = ps[x] * ps[y] * ps[z] * ps[w];
        std::vector <std::size_t> mults {ps[x], ps[y], ps[z], ps[w]}; 
        std::sort (mults.begin (), mults.end ());

        auto res = solve2 (num);
        if (res.size () != mults.size () ||
            !std::equal (res.cbegin (), res.cend (), mults.cbegin ())) {
            std::cout << "num: " << num << '\n';
            std::cout << "ref: " << mults <<  '\n';
            std::cout << "res: " << res << '\n';
            std::cout << "F\n";
            return;
        }
    }}}}
}

int main () {
    std::ios_base::sync_with_stdio (false);

    std::size_t n = 0;
    std::cin >> n;
//    test (n);
    std::cout << solve2 (n) << std::endl;
}
