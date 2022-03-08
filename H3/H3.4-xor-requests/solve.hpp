#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>

// g++ -DHOST -std=c++17 main.cpp

#define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define dump(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define dump(obj)
#endif

class xor_array {
    std::vector <unsigned> arr;

public:
    xor_array (std::size_t size_arr, std::istream& is) :
        arr (size_arr)
    {
        for (auto& elem : arr) {
            is >> elem;
            if (is.fail ()) {
                throw std::invalid_argument ("Failed read input data");
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

    void
    dump_arr (std::ostream& os = std::cout) const {
        for (std::size_t i = 0; i < arr.size (); ++i) {
            std::cout << i << ": " << arr[i] << "\n";
        }
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