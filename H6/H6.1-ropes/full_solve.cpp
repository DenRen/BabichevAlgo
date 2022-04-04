#include <iostream>
#include <algorithm>

// g++ -DHOST -std=c++17 main.cpp

#define NDEBUG
// #define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define DUMP(obj)
#endif

std::vector <int>
read_nums (std::istream& is, int N) {
    std::vector <int> nums (N);
    for (int i = 0; i < N; ++i) {
        is >> nums[i];
    }

    return nums;
}

int solve (std::vector <int>& nums) {
    if (nums.size () <= 1) {
        return 0;
    }

    std::sort (nums.begin (), nums.end ());

    if (nums.size () == 2) {
        return nums[1] - nums[0];
    }

    int dist_prev = nums[1] - nums[0];
    int dist_last = nums[2] - nums[0];
    int num_last = nums[2];

    for (int i = 3; i < nums.size (); ++i) {
        int num = nums[i];
        int dist = std::min (dist_prev, dist_last) + num - num_last;

        num_last = num;
        dist_prev = dist_last;
        dist_last = dist;
    }

    return dist_last;
}

int
solve (std::istream& is, int N) {
    auto nails = read_nums (is, N);
    return solve (nails);
}

int main () {
    std::ios_base::sync_with_stdio (false);

    int N = 0;
    std::cin >> N;
    std::cout << solve (std::cin, N) << std::endl;
}