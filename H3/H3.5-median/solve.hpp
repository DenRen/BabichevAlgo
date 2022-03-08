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

class median_array {
public:
    typedef std::size_t value_type;

    median_array (std::size_t size_arr, std::istream& is) :
        arr (size_arr)
    {
        for (auto& elem : arr) {
            is >> elem;
            if (is.fail ()) {
                throw std::invalid_argument ("Failed read input data");
            }
        }
    }

    value_type
    median () {
        return 4;
    }

    void
    add (value_type value) {

    }

    void
    dump_arr (std::ostream& os = std::cout) const {
        for (std::size_t i = 0; i < arr.size (); ++i) {
            std::cout << i << ": " << arr[i] << "\n";
        }
    }

private:
    std::vector <value_type> arr;
};

std::pair <median_array, std::size_t>
read_input (std::istream& is = std::cin)
{
    std::size_t size_arr = 0, num_req = 0;
    is >> size_arr >> num_req;
    if (is.fail ()) {
        throw std::invalid_argument ("Read size_arr failed");
    }

    return {std::move (median_array {size_arr, is}), num_req};
}

void
exec_requests (median_array& arr,
               std::size_t num_req,
               std::istream& is = std::cin,
               std::ostream& os = std::cout)
{
    while (num_req--) {
        int type = 0;
        is >> type;

        switch (type) {
        case 0: {
            median_array::value_type value = 0;
            is >> value;
            arr.add (value);
        } break;

        case 1: {
            os << arr.median () << "\n";
        } break;

        default:
            throw std::invalid_argument ("Incorrect request type");
            break;
        }
    }
}