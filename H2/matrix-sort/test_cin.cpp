#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

#include "../../libs/print_lib.hpp"

int main () {
    std::size_t N = 0;
    std::cin >> N;
    
    std::vector <int> vec;
    vec.reserve (N);

    std::istream_iterator <int> it_cin {std::cin};
    std::cout << "lol" << std::endl;
    for (int i = 0; i < N; ++i) {
        vec.push_back (*it_cin);
        if (i != N - 1) {
            it_cin++;
        }
    }

    std::cout << vec << std::endl;
}