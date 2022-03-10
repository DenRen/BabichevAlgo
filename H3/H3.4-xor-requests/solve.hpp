#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <stdio.h>

// g++ -std=c++17 -D_GLIBCXX_DEBUG main.cpp -g3
// g++ -DHOST -std=c++17 main.cpp

// #define NDEBUG
#define HOST

#ifdef HOST
    #include "../../libs/other_func.hpp"
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define dump(obj)
#endif

namespace intmath {
    inline bool
    is_pow2 (std::size_t num) noexcept {
        return (num & (num - 1)) == 0;
    }

    inline std::size_t
    log2 (std::size_t n) noexcept {
        return 64 - __builtin_clzl (n);
    }

}

template <typename T>
class xor_array_native {
    std::vector <T> arr;

public:
    xor_array_native (const std::vector <T>& vec) :
        arr (vec)
    {}

    T
    xor_range (std::size_t L, std::size_t R) {
        assert (L < arr.size ());
        assert (R < arr.size ());
        assert (L <= R);

        T res = arr[L];
        for (auto i = L + 1; i <= R; ++i) {
            res ^= arr[i];
        }
        return res;
    }

    void
    update (std::size_t pos, T value) {
        assert (pos < arr.size ());
        arr[pos] = value;
    }

    std::ostream&
    dump_arr (std::ostream& os = std::cout) const {
        if (arr.size () == 0) {
            return os;
        }

        os << arr[0];
        for (std::size_t i = 1; i < arr.size (); ++i) {
            os << " " << arr[i];
        }

        return os;
    }
};

template <typename T>
class xor_array {
    std::size_t size;
    std::size_t cap;
    std::vector <T> arr;

    inline std::size_t
    num_lvls () const noexcept {
        return intmath::log2 (cap);
    }

    inline std::size_t
    calc_arr_cap (std::size_t num_elems) {
        const auto msb = intmath::log2 (num_elems) - 1;
        const bool exist_other_bits = !intmath::is_pow2 (num_elems);
        return (1 << (msb + exist_other_bits));
    }

    inline std::size_t
    calc_arr_size (std::size_t cap) const noexcept {
        return 2 * cap - 1;
    }

    void
    init_bin_heap () noexcept {
        std::size_t c = cap;

        while (c != 0) {
            std::size_t j = c - 1;
            c /= 2;
            for (std::size_t i = c - 1; i < 2 * c - 1; ++i) {
                arr[i] = arr[j] ^ arr[j + 1];
                j += 2;
            }
        }
    }

    inline std::size_t
    begin_index () const noexcept {
        return cap - 1;
    }

    inline std::size_t
    end_index () const noexcept {
        return begin_index () + size;
    }

    xor_array (std::size_t size_arr) :
        size (size_arr),
        cap (calc_arr_cap (size_arr)),
        arr (calc_arr_size (cap))
    {}

    T
    xor_range_impl (std::size_t global_L, std::size_t global_R) const {
        T res = 0;
        auto& L = global_L, R = global_R;

        if (L % 2 == 1) res ^= arr[L++ - 1];
        if (R % 2 == 0) res ^= arr[R-- - 1];
        if (R > L) res ^= xor_range_impl (L / 2, R / 2);

        return res;
    }
public:
    xor_array (const std::vector <T>& vec) :
        xor_array {vec.size ()}
    {
        const auto begin = begin_index ();
        const auto end = end_index ();
        for (std::size_t i = begin; i < end; ++i) {
            arr[i] = vec[i - begin];
        }

        init_bin_heap ();
    }

    xor_array (std::size_t size_arr, std::istream& is) :
        xor_array {size_arr}
    {
        const auto end = end_index ();
        for (std::size_t i = begin_index (); i < end; ++i) {
            is >> arr[i];
        }

        if (is.fail ()) {
            throw std::invalid_argument ("Failed read input data");
        }

        init_bin_heap ();
    }

    T
    xor_range (std::size_t L, std::size_t R) const {
        assert (L < size);
        assert (R < size);
        assert (L <= R);

        const auto begin = 1 + begin_index ();
        return xor_range_impl (begin + L, begin + R);
    }

    void
    update (std::size_t pos, T value) {
        assert (pos < size);

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

template <typename T>
std::pair <xor_array <T>, std::size_t>
read_input (std::istream& is = std::cin)
{
    std::size_t size_arr = 0, num_req = 0;
    is >> size_arr >> num_req;
    if (is.fail ()) {
        throw std::invalid_argument ("Read size_arr failed");
    }

    return {std::move (xor_array <T> {size_arr, is}), num_req};
}

template <typename T>
void
exec_requests (xor_array <T>& arr,
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
            T new_value = 0;
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