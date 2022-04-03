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
#include <unordered_map>

// g++ -DHOST -std=c++17 main.cpp

#define NDEBUG
// #define HOST

#ifdef HOST 
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define DUMP(obj)
#endif

std::vector <std::size_t>
read_req (std::istream& is, unsigned num_req) {
    std::vector <std::size_t> req (num_req);
    for (unsigned i = 0; i < num_req; ++i) {
        is >> req[i];
    }

    return req;
}

std::unordered_map <std::size_t, std::vector <unsigned>>
get_request_positions (std::vector <std::size_t>& req, unsigned num_req) {
    std::unordered_map <std::size_t, std::vector <unsigned>> req_poss;
    for (unsigned i = 0; i < num_req; ++i) {
        req_poss[req[i]].push_back (i);
    }

    return req_poss;
}

unsigned
solve (std::istream& is, unsigned size_cache, unsigned num_req) {
    auto req_arr = read_req (is, num_req);
    auto req_pos_table = get_request_positions (req_arr, num_req);

    unsigned num_system_req = 0, i_req = 0;
    std::set <std::size_t> cache;
    for (; i_req < num_req && cache.size () < size_cache; ++i_req) {
        auto req = req_arr[i_req];
        auto it = cache.find (req);
        if (it == cache.end ()) {
            ++num_system_req;
            cache.insert (req);
        }
    }
    if (i_req == num_req) {
        return num_system_req;
    }

    for (; i_req < num_req; ++i_req) {
        auto req = req_arr[i_req];

        // Check req in cache
        auto it = cache.find (req);
        if (it != cache.end ()) {
            continue;
        }

        // Req is miss in cache
        // Find the max distance between cache item and it next pos
        unsigned max_pos = i_req;
        std::size_t selected_item_cache = 0;
        for (const auto& req : cache) {
            auto& poss = req_pos_table.find (req)->second;
            
            unsigned pos = 0;
            auto it = std::lower_bound (poss.begin (), poss.end (), i_req);
            if (it == poss.end ()) {
                selected_item_cache = req;
                break;
            } else {
                if (*it > max_pos) {
                    max_pos = *it;
                    selected_item_cache = req;
                }
            }
        }

        ++num_system_req;
        cache.erase (selected_item_cache);
        cache.insert (req);        
    }

    return num_system_req;
}

int main () {
    std::ios_base::sync_with_stdio (false);

    std::size_t size_cache = 0, num_req = 0;
    std::cin >> size_cache >> num_req;
    std::cout << solve (std::cin, size_cache, num_req) << '\n';
}