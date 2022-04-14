#include <gtest/gtest.h>
#include "solve.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

int
line_bin_find (int x, int n, int k) {
    if (k > 32) {
        k = 32;
    }

    std::size_t step = 1 << (k - 1);
    int i_nd = 1, step = n;
    
    if (k > 64) {
        k = 64;
    }
    
    while (i_d - i_nd + 1 > step) {
        
    }
}

int
native_solve (int n, int k) {
    for (int x = 1; x <= n; ++x) {
                
    }
}

TEST (_, _) {

}
