#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

template <typename T>
std::istream& operator >> (std::istream& is, std::vector <T>& vec) {
    T value;

    is >> value;
    while (value != 0) {
        vec.push_back (value);
        is >> value;
    }

    return is;
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

template <typename T>
std::vector <T> solve (std::vector <T>& a, std::vector <T>& b) {
    std::sort (a.begin (), a.end ());
    std::sort (b.begin (), b.end ());

    if (a.size () == 0) {
        return b;
    } else if (b.size () == 0) {
        return a;
    }

    // Here a.size () > 0 && b.size () > 0
    std::vector <T> res;
    res.reserve (a.size () + b.size ());

    auto iter_a = a.cbegin (), iter_b = b.cbegin ();
    while (iter_a != a.cend () && iter_b != b.cend ()) {
        if (*iter_a < *iter_b) {
            res.push_back (*iter_a++);
        } else if (*iter_a == *iter_b) {
            ++iter_a;
            ++iter_b;
        } else {
            res.push_back (*iter_b++);
        }
    }

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

int main () {
    const std::size_t init_size = 128;
    std::vector <uint32_t> a, b;
    a.reserve (init_size);
    b.reserve (init_size);

    std::cin >> a >> b;

    std::cout << solve (a, b);
}
