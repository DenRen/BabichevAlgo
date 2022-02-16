#include <iostream>
#include <algorithm>
#include <vector>

#include "solve.hpp"

int main () {
    std::vector <int> points = get_vector <int> ();
    int min_len = solve (points);
    std::cout << min_len << std::endl;
}