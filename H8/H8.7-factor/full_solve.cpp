#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cmath>

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

std::vector <std::size_t>
solve2 (std::size_t n) {
    std::vector <std::size_t> mults;
    
    while ((n & 1) == 0) {
        mults.push_back (2);
        n /= 2;
    }

    std::size_t root_n = std::sqrt (n);
    for (std::size_t i = 3; i <= root_n; i += 2) {
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

int main () {
    std::size_t n = 0;
    std::cin >> n;
    std::cout << solve2 (n) << std::endl;
}
