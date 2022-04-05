#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <map>
#include <queue>
#include <set>
#include <iomanip>
#include <stack>
#include <fstream>
#include <cmath>

// g++ -DHOST -std=c++17 main.cpp

// #define NDEBUG
#define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define DUMP(obj)
#endif

uint64_t
F (int N) {
    return 2 * (((uint64_t) 1 << N) - 1);
}

uint64_t
pow2 (unsigned n) {
    return (uint64_t) 1 << n;
}

uint64_t
f (int pos, std::vector <int>& nums) {
    if (pos == nums.size ()) return 1;

    int left = nums[pos - 1];
    int right = nums[pos];
    int m = nums.size () - pos;

    if (left <= 2) return 0;
    if (left == 3) {
        if (right <= 2) return 0;
        if (right == 3) return f (pos + 1, nums);
        if (right <= 6) return pow2 (m - 1);
        if (right == 7) return pow2 (m - 1) + f (pos + 1, nums);
        else            return pow2 (m);
    }
    if (left <= 6) {
        throw std::runtime_error ("4,5,6 fff");
    }
    if (left == 7) {
        if (right <= 2) return 0;
        if (right == 3) return f (pos + 1, nums);
        if (right <= 6) return pow2 (m - 1);
        if (right == 7) return pow2 (m - 1) + f (pos + 1, nums);
        else            return pow2 (m);
    }
    return pow2 (m);
}

uint64_t
f_first (int left, std::vector <int>& nums) {
    if (nums.size () == 1) {
        if (left <= 2) return 0;
        if (left <= 6) return 1;
        else           return 2;
    }

    int n = nums.size ();
    int right = nums[1];

    if (left <= 2) return F (n - 1);
    if (left == 3) {
        if (right <= 2) return F (n - 1);
        if (right == 3) return F (n - 1) + f (2, nums);
        if (right <= 6) return F (n - 1) + pow2 (n - 2);
        if (right == 7) return F (n - 1) + pow2 (n - 2) + f (2, nums);
        else            return F (n - 1) + pow2 (n - 1);
    }
    if (left <= 6) return F (n - 1) + pow2 (n - 1);
    if (left == 7) {
        if (right <= 2) return F (n - 1) + pow2 (n - 1);
        if (right == 3) return F (n - 1) + pow2 (n - 1) + f (2, nums);
        if (right <= 6) return F (n - 1) + pow2 (n - 1) + pow2 (n - 2);
        if (right == 7) return F (n - 1) + pow2 (n - 1) + pow2 (n - 2) + f (2, nums);
        else            return F (n);
    }
    
    return F (n);
}

uint64_t
F_first (std::vector <int>& nums) {
    return f_first (nums[0], nums);
}

uint64_t
solve (std::vector <int>& nums) {
    return F_first (nums);
}