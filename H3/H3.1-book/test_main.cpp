#include <vector>
#include <algorithm>

#include "../../libs/print_lib.hpp"

int main () {
    std::vector <int> v {12, 344, 657, 777};
    std::sort (v.begin (), v.end (), std::greater <> ());
    std::cout << v << std::endl;
    
    const int val = 11;
    auto it = std::lower_bound (v.begin (), v.end (), val, std::greater <> ());
    std::cout << it - v.cbegin () << std::endl;

    v.insert (it, val);
    std::cout << v << std::endl;
}
