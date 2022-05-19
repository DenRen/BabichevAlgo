#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector <T>& vec) {
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
std::pair <T, T> up_fact (T number) {
    std::size_t res = 1, n = 1;
    while (number > res) {
        res *= ++n;
    }

    return {res, n};
}

template <typename T>
std::vector <T> solve (const T N, T M) {
    std::vector <T> res;
    res.reserve (N);
    for (std::size_t i = 1; i <= N; ++i) {
        res.push_back (i);
    }

    auto [power, n] = up_fact (M--);
    for (auto pos = res.begin () + N - n; pos != res.end (); ++pos) {
        power /= n--;
        T num = M / power;

        std::swap (*pos, *(pos + num));
        std::sort (pos + 1, res.end ());

        M -= num * power;
        if (M == 0) {
            break;
        }
    }

    return res;
}

int main () {
    std::size_t N = 0, M = 0;
    std::cin >> N >> M;

    std::cout << solve (N, M);
}
