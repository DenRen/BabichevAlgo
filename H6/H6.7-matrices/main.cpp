#include <iostream>
#include <vector>

std::vector <int>
read_nums (std::istream& is = std::cin) {
    int N = 0;
    is >> N;

    std::vector <int> nums (N);
    for (auto& num : nums) {
        is >> num;
    }

    return nums;
}

std::size_t
calc_sum (const std::vector <int>& nums,
          std::vector <std::vector <std::size_t>>& sum_arr,
          int i,
          int j)
{
    if (i == j) {
        return 0;
    }

    auto& sum_arr_item = sum_arr[i][j];
    if (sum_arr_item != -1) {
        return sum_arr_item;
    }

    std::size_t min_sum = -1;
    for (int k = i; k < j; ++k) {
        std::size_t sum = calc_sum (nums, sum_arr, i, k) +
                          calc_sum (nums, sum_arr, k + 1, j) +
                          (std::size_t) nums[i] * nums[k + 1] * nums[j + 1];
        if (sum < min_sum) {
            min_sum = sum;
        }
    }

    return sum_arr_item = min_sum;
}

std::size_t
solve (std::vector <int>& nums) {
    std::size_t N = nums.size ();

    std::vector <std::vector <std::size_t>> sum_arr (N - 1);
    for (auto& sum_row : sum_arr) {
        sum_row = std::vector <std::size_t> (N - 1, -1);
    }

    return calc_sum (nums, sum_arr, 0, N - 2);
}

int main () {
    std::ios_base::sync_with_stdio (false);

    auto nums = read_nums ();

    std::cout << solve (nums) << '\n';
    return 0;
}































