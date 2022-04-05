#include "solve.hpp"

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