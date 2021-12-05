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

int main () {
    std::string set;
    std::cin >> set;


}
