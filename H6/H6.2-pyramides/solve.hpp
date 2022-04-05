#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <map>
#include <queue>
#include <set>
#include <iomanip>
#include <stack>
#include <fstream>

// g++ -DHOST -std=c++17 main.cpp

// #define NDEBUG
#define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define DUMP(obj)
#endif

typedef std::size_t T;

class min_base_t {
    //         N    n
    mutable std::map <int, int> N_buf;

    void
    fill_buf (int N) const {
        N = std::max (N, 10);

        int prev_N = 0;
        for (int n = 0; n * n <= N; ++n) {
            prev_N += n;
            N_buf[prev_N] = n;
        }
    }

public:
    min_base_t (int N = 255*255) {
        fill_buf (N);
    }

    int
    get_base (int N) const {
        auto it = N_buf.lower_bound (N);
        if (it == N_buf.end ()) {
            std::cout << "lol\n";
            fill_buf (N);
            return N_buf.lower_bound (N)->second;
        }

        return it->second;
    }
};

class F_t {
    struct key_t {
        int len_base;
        int N;

        key_t (int len_base, int N) :
            len_base (len_base),
            N (N)
        {}

        bool
        operator < (const key_t& rhs) const noexcept {
            return len_base < rhs.len_base ||
                   (len_base == rhs.len_base && N < rhs.N);
        }
    };

    std::map <key_t, int> cache;
    min_base_t min_base;

public:
    T
    operator () (int l, int N) {
        if (l == 0 || l == N) {
            return 1;
        }

        auto it = cache.find ({l, N});
        if (it != cache.end ()) {
            // std::cout << l << ' ' << N << '\n';
            // std::cout << it->first.len_base << ' ' << it->first.N << '\n';
            // std::cout << "F(" << l << ", " << N << ") = " << it->second << "\n";
            return it->second;
        }

        T res = 0;
        int n = min_base.get_base (N - l);
        int l_max = std::min (l - 1, N - l);

        for (int l_ = n; l_ <= l_max; ++l_) {
            res += (*this) (l_, N - l);
        }

        key_t key {l, N};
        cache.emplace (key, res);

        // std::cout << "F(" << l << ", " << N << ") = " << res << "\n";
        return res;
    }

    T
    operator () (int N) {
        // std::cout << "F(" << N << "):\n";
        T res = 0;
        int n = min_base.get_base (N);
        for (int l = n; l <= N; ++l) {
            res += (*this) (l, N);
        }

        return res;
    }
};

T
solve (int N) {
    F_t F;
    // std::cout << "F(" << N << "): ";
    return F (N);
}