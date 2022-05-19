#include <iostream>
#include <cstdint>

int main () {
    uint32_t N = 0;
    std::cin >> N;

    uint32_t min_add_sum_5 = 0;
    uint32_t cur_sum_5 = 0;

    for (uint32_t i = 0; i < N; ++i) {
        uint32_t cur_bill = 0;
        std::cin >> cur_bill;

        if (cur_bill == 5) {
            cur_sum_5 += 5;
        } else {
            uint32_t change = cur_bill - 5;
            if (change > cur_sum_5) {
                min_add_sum_5 += change - cur_sum_5;
                cur_sum_5 = 0;
            } else {
                cur_sum_5 -= change;
            }
        }
    }

    std::cout << min_add_sum_5 / 5 << std::endl;
}
