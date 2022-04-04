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

int
find_pos_max_dist (const std::vector <int>& dists,
                   int l,
                   int r) {
    int max_dist = 0, pos_max_dist = 0;
    for (++l; l < r - 1; ++l) {
        if (dists[l] > max_dist) {
            max_dist = dists[l];
            pos_max_dist = l;
        }
    }
    return pos_max_dist;
}

// [l, r), r - l > 1
int
get_len_ropes (const std::vector <int>& dists,
               int l,
               int r) {
    assert (r - l);
    int num_nails = r - l;
    if (num_nails == 1) {
        return dists[l];
    }
    if (num_nails == 2) {
        return dists[l] + dists[l + 1];
    }

    // num_dists >= 3
    int pos_max_dist = find_pos_max_dist (dists, l, r);
    int len = get_len_ropes (dists, l, pos_max_dist) +
              get_len_ropes (dists, pos_max_dist + 1, r);

    return len;
}

// nums sorted
// dists[0] = nums[1] - nums[0]
std::vector <int>&
nums2dists (std::vector <int>& nums) {
    const auto size = nums.size ();
    for (std::size_t i = 0; i < size - 1; ++i) {
        nums[i] = nums[i + 1] - nums[i];
    }
    nums.resize (size - 1);

    return nums;
}

int solve (std::vector <int>& nails) {
    if (nails.size () <= 1) {
        return 0;
    }

    std::sort (nails.begin (), nails.end ());
    auto& dists = nums2dists (nails);

    return get_len_ropes (dists, 0, nails.size ());
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