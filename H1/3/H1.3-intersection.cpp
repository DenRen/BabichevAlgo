#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

// a.size () >= 2 && b.size () >= 2
template <typename T>
std::vector <T> solve (std::vector <T>& a, std::vector <T>& b) {
    std::vector <T> res;
    res.reserve (a.size () + b.size ());

    auto iter_a = a.cbegin (), iter_b = b.cbegin ();
    while (iter_a != a.cend () && iter_b != b.cend ()) {
        if (*iter_a < *iter_b) {
            ++iter_a;
        } else if (*iter_a == *iter_b) {
            res.push_back (*iter_a);
            ++iter_a;
            ++iter_b;
        } else {
            ++iter_b;
        }
    }

    return res;

    if (iter_a == a.cend () && iter_b == b.cend ()) {
        return res;
    }

    if (iter_a != a.cend ()) {
        do {
            res.push_back (*iter_a++);
        } while (iter_a != a.cend ());
    } else {
        do {
            res.push_back (*iter_b++);
        } while (iter_b != b.cend ());
    }

    return res;
}

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

int main () {
    std::size_t size_a = 0, size_b = 0;
    std::cin >> size_a >> size_b;

    std::vector <uint32_t> a, b;
    a.reserve (size_a);
    b.reserve (size_b);

    decltype (a)::value_type value = 0;
    for (std::size_t i = 0; i < size_a; ++i) {
        std::cin >> value;
        a.push_back (value);
    }
    for (std::size_t i = 0; i < size_b; ++i) {
        std::cin >> value;
        b.push_back (value);
    }

    std::cout << solve (a, b);
}
