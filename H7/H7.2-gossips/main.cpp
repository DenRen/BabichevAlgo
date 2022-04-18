//#include "solve.hpp"

#include <iostream>
#include <vector>
#include <array>
#include <queue>

struct coord_t {
    int x, y;

    coord_t (int x = 0, int y = 0) :
        x (x), y (y)
    {}
};

int
solve (const int x_size,
       const int y_size,
       std::vector <coord_t>& inits_arr) {
    
    std::vector <std::vector <bool>> graph (y_size);
    for (auto& row : graph) {
        row = std::vector <bool> (x_size, false);
    }
    
    for (const auto&[x, y] : inits_arr) {
        graph[y][x] = true;
    }

    std::vector <coord_t> next_arr;

    auto* next_ptr = &next_arr, *prev_ptr = &inits_arr;
    
    auto add_next = [&] (int x, int y, int off_x, int off_y) {
        int neigh_x = x + off_x, neigh_y = y + off_y;

        if (neigh_x >= 0 && neigh_x < x_size &&
            neigh_y >= 0 && neigh_y < y_size &&
            !graph[neigh_y][neigh_x])
        {
            graph[neigh_y][neigh_x] = true;
            next_ptr->push_back ({neigh_x, neigh_y});
        }
    };

    auto add_next_simple = [&] (int x, int y, int off_x, int off_y) {
        int neigh_x = x + off_x, neigh_y = y + off_y;

        if (!graph[neigh_y][neigh_x])
        {
            graph[neigh_y][neigh_x] = true;
            next_ptr->push_back ({neigh_x, neigh_y});
        }
    };

    int time = -1;
    while (prev_ptr->size ()) {
        auto& next = *next_ptr;
        auto& prev = *prev_ptr;

        for (const auto&[x, y] : prev) {
            if (x > 0 && x + 1 < x_size &&
                y > 0 && y + 1 < y_size) {
                add_next_simple (x, y,  1,  0);
                add_next_simple (x, y, -1,  0);
                add_next_simple (x, y,  0,  1);
                add_next_simple (x, y,  0, -1);
            } else {
                add_next (x, y,  1,  0);
                add_next (x, y, -1,  0);
                add_next (x, y,  0,  1);
                add_next (x, y,  0, -1);
            }
        }
        
        prev.clear ();
        std::swap (next, prev);
        ++time;
    }
    
    return time;
}


int main () {
    std::ios_base::sync_with_stdio (false);

    int N = 0, M = 0, K = 0;
    std::cin >> N >> M >> K;

    std::vector <coord_t> inits (K);
    for (auto& initiator : inits) {
        std::cin >> initiator.x >> initiator.y;
    }

    std::cout << solve (N, M, inits) << '\n';
}
