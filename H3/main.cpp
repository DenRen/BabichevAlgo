#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>

// g++ -DHOST -std=c++17 main.cpp

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#endif

int main () {

}
