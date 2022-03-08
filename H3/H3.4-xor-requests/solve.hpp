#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <stdio.h>
// g++ -DHOST -std=c++17 main.cpp

#define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define dump(obj)
#endif

class xor_array {
    std::size_t size;
    std::size_t cap;
    std::vector <unsigned> arr;

    inline bool
    is_pow2 (std::size_t num) const noexcept {
        return (num & (num - 1)) == 0;
    }

    inline std::size_t
    log2 (std::size_t n) const noexcept {
        return 64 - __builtin_clzl (n);
    }

    inline std::size_t
    num_lvls () const noexcept {
        return log2 (cap);
    }

    inline std::size_t
    calc_arr_cap (std::size_t num_elems) {
        return (1 << (log2 (num_elems) - 1 + !is_pow2 (num_elems)));
    }

    inline unsigned
    calc_arr_size (std::size_t cap) const noexcept {
        return 2 * cap - 1;
    }

public:
    xor_array (std::size_t size_arr, std::istream& is) :
        size (size_arr),
        cap (calc_arr_cap (size_arr)),
        arr (calc_arr_size (cap))
    {
        for (std::size_t i = cap - 1; i < cap - 1 + size; ++i) {
            is >> arr[i];
        }

        if (is.fail ()) {
            throw std::invalid_argument ("Failed read input data");
        }

        std::size_t c = cap;
        while (c != 0) {
            std::size_t j = c - 1;
            c /= 2;
            for (std::size_t i = c - 1; i < 2 * c - 1; ++i) {
                arr[i] = arr[j] + arr[j + 1];
                j += 2;
            }
        }
    }

    unsigned
    xor_range (std::size_t L, std::size_t R) {
        assert (L < arr.size ());
        assert (R < arr.size ());
        assert (L <= R);

        return 0;
    }

    void
    update (std::size_t pos, unsigned value) {
        assert (pos < arr.size ());

        arr[pos] = value;
    }

    std::ostream&
    dump_arr (std::ostream& os = std::cout) const {
        for (std::size_t size_lvl = 1; size_lvl <= cap; size_lvl *= 2) {
            std::size_t begin = size_lvl - 1;
            std::size_t end = 2 * size_lvl - 1;

            os << arr[begin];
            for (std::size_t i = begin + 1; i < end; ++i) {
                os << " " << arr[i];
            }
            os << "\n";
        }

        return os;
    }
};

std::pair <xor_array, std::size_t>
read_input (std::istream& is = std::cin)
{
    std::size_t size_arr = 0, num_req = 0;
    is >> size_arr >> num_req;
    if (is.fail ()) {
        throw std::invalid_argument ("Read size_arr failed");
    }

    return {std::move (xor_array {size_arr, is}), num_req};
}

void
exec_requests (xor_array& arr,
               std::size_t num_req,
               std::istream& is = std::cin,
               std::ostream& os = std::cout)
{
    while (num_req--) {
        int type = 0;
        is >> type;

        switch (type) {
        case 1: {
            std::size_t L = 0, R = 0;
            is >> L >> R;
            if (is.fail ()) {
                throw std::invalid_argument ("Failed read L or R");
            }

            os << arr.xor_range (L, R) << "\n";
        } break;

        case 2: {
            std::size_t pos = 0;
            unsigned new_value = 0;
            is >> pos >> new_value;
            if (is.fail ()) {
                throw std::invalid_argument ("Failed read pos or new_value");
            }

            arr.update (pos, new_value);
        } break;

        default:
            throw std::invalid_argument ("Incorrect request type");
            break;
        }
    }
}