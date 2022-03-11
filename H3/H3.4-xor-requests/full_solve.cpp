#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <cstdio>

// g++ -std=c++17 -D_GLIBCXX_DEBUG main.cpp -g3
// g++ -DHOST -std=c++17 main.cpp

#define NDEBUG

namespace intmath {
    inline bool
    is_pow2 (std::size_t num) noexcept {
        return (num & (num - 1)) == 0;
    }

    template <typename T>
    inline T
    log2 (T n) noexcept {
        if constexpr (sizeof (T) <= 4) {
            return 32 - __builtin_clz (n);
        } else {
            return 64 - __builtin_clzl (n);
        }
    }
}

template <typename T, typename SizeT = unsigned>
class xor_array {
    SizeT size;
    SizeT cap;
    std::vector <T> arr;

    inline SizeT
    num_lvls () const noexcept {
        return intmath::log2 (cap);
    }

    inline SizeT
    calc_arr_cap (SizeT num_elems) {
        const auto msb = intmath::log2 (num_elems) - 1;
        const bool exist_other_bits = !intmath::is_pow2 (num_elems);
        return (1 << (msb + exist_other_bits));
    }

    inline SizeT
    calc_arr_size (SizeT cap) const noexcept {
        return 2 * cap;
    }

    void
    init_bin_heap () noexcept {
        auto c = cap;

        while (c != 0) {
            auto j = c;
            c /= 2;
            for (auto i = c; i < 2 * c; ++i) {
                arr[i] = arr[j] ^ arr[j + 1];
                j += 2;
            }
        }
    }

    inline SizeT
    begin_index () const noexcept {
        return cap;
    }

    inline SizeT
    end_index () const noexcept {
        return begin_index () + size;
    }

    xor_array (SizeT size_arr) :
        size (size_arr),
        cap (calc_arr_cap (size_arr)),
        arr (calc_arr_size (cap))
    {}

    T
    xor_range_impl (SizeT global_L, SizeT global_R) const {
        T res = 0;
        auto& L = global_L, R = global_R;
        
        if (L % 2 == 1) res ^= arr[L++];
        if (R % 2 == 0) res ^= arr[R--];

        while (R > L) {
            L /= 2;
            R /= 2;

            if (L % 2 == 1) res ^= arr[L++];
            if (R % 2 == 0) res ^= arr[R--];
        }

        return res;
    }
public:
    xor_array (const std::vector <T>& vec) :
        xor_array {vec.size ()}
    {
        const auto begin = begin_index ();
        const auto end = end_index ();
        for (auto i = begin; i < end; ++i) {
            arr[i] = vec[i - begin];
        }

        init_bin_heap ();
    }

    xor_array (SizeT size_arr, std::istream& is) :
        xor_array {size_arr}
    {
        const auto end = end_index ();
        for (auto i = begin_index (); i < end; ++i) {
            is >> arr[i];
        }

        if (is.fail ()) {
            throw std::invalid_argument ("Failed read input data");
        }

        init_bin_heap ();
    }

    T
    xor_range (SizeT L, SizeT R) const {
        assert (L < size);
        assert (R < size);
        assert (L <= R);

        const auto begin = begin_index ();
        return xor_range_impl (begin + L, begin + R);
    }

    void
    update (SizeT pos, T value) {
        assert (pos < size);

        const auto begin = pos + begin_index ();
        arr[begin] = value;

        for (auto i = begin / 2; i != 0; i /= 2) {
            arr[i] = arr[2 * i] ^ arr[2 * i + 1];
        }
    }

    std::ostream&
    dump_arr (std::ostream& os = std::cout) const {
        for (std::size_t size_lvl = 1; size_lvl <= cap; size_lvl *= 2) {
            SizeT begin = size_lvl;
            SizeT end = 2 * size_lvl;

            os << arr[begin];
            for (std::size_t i = begin + 1; i < end; ++i) {
                os << " " << arr[i];
            }
            os << "\n";
        }

        return os;
    }
};

template <typename T, typename SizeT>
std::pair <xor_array <T, SizeT>, SizeT>
read_input (std::istream& is = std::cin)
{
    SizeT size_arr = 0, num_req = 0;
    is >> size_arr >> num_req;
    if (is.fail ()) {
        throw std::invalid_argument ("Read size_arr failed");
    }

    return {std::move (xor_array <T, SizeT> {size_arr, is}), num_req};
}

template <typename T, typename SizeT>
void
exec_requests (xor_array <T, SizeT>& arr,
               SizeT num_req,
               std::istream& is = std::cin,
               std::ostream& os = std::cout)
{
    while (num_req--) {
        int type = 0;
        scanf ("%d", &type);

        switch (type) {
        case 1: {
            SizeT L = 0, R = 0;
            if (scanf ("%u %u", &L, &R) != 2) {
                throw std::invalid_argument ("Failed read L or R");
            }

            printf ("%d\n", arr.xor_range (L, R));
        } break;

        case 2: {
            SizeT pos = 0;
            T new_value = 0;

            if (scanf ("%u %d", &pos, &new_value) != 2) {
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

int main () {
    auto[arr, num_req] = read_input <int, unsigned> ();
    exec_requests (arr, num_req);
}