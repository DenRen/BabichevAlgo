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

#define NDEBUG
// #define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define DUMP(obj)
#endif

std::size_t
F (int N) {
    return 2 * ((1 << N) - 1);
}

std::size_t
F_middle (int pos, std::vector <int>& nums) {
    std::size_t N = nums.size ();

    if (pos == N) {
        return 1;
    }
    assert (pos < N);

    int num = nums[pos];
    if (num <= 2) {
        return 0;
    }
    if (num == 3) {
        return F_middle (pos + 1, nums);
    }

    int n = N - pos;
    if (num <= 6) {
        return 1 << (n - 1);
    }
    
    if (num == 7) {
        return (1 << (n - 1)) + F_middle (pos + 1, nums);
    }

    return 1 << n;
}

std::size_t
solve (std::vector <int>& nums) {
    auto N = nums.size ();
    if (nums[0] >= 8) {
        return F (N);
    }

    if (nums[0] == 0) {
        return 0;
    }

    if (nums[0] <= 2) {
        return F (nums.size () - 1);
    }

    if (nums[0] == 7) {
        if (N == 1) {
            return 2;
        }
        return F (N - 1) + (1 << (N - 1)) + F_middle (1, nums);
    }

    // (3, 6]
    if (nums[0] > 3) {
        if (N == 1) {
            return 1;
        }
        return F (N - 1) + (1 << (N - 1));
    }

    // 3
    return F (N - 1) + F_middle (1, nums);
}

int main () {
    std::ios_base::sync_with_stdio (false);

    std::string str;
    std::getline (std::cin, str);

    int n = str.size ();
    std::vector <int> nums (n);
    for (int i = 0; i < n; ++i) {
        nums[i] = str[i] - '0';
    }

    std::cout << solve (nums) << '\n';
}