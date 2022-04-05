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

// g++ -DHOST -std=c++17 main.cpp

// #define NDEBUG
// #define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cout << #obj ": " << obj << "\n"
#else
    #define DUMP(obj)
#endif

struct coord_t {
    int x, y;

    coord_t (int x = 0, int y = 0) :
        x (x), y (y)
    {}

    bool
    operator == (const coord_t& rhs) const noexcept {
        return x == rhs.x && y == rhs.y;
    }
};

std::tuple <std::vector <std::vector <bool>>, coord_t, coord_t>
read_lab (std::istream& is = std::cin) {
    int N = 0, M = 0, K = 0;
    is >> N >> M >> K;

    std::vector <std::vector <bool>> lab (N);
    coord_t start, finish;

    std::string row;
    for (int i = 0; i < N; ++i) {
        do {
            std::getline (is, row);
        } while (row.size () == 0);

        lab[i].resize (M);
        auto& lab_row = lab[i];
        for (int j = 0; j < M; ++j) {
            switch (row[j]) {
                case '+': {
                    lab_row[j] = false;
                } break;
                case '.': {
                    lab_row[j] = true;
                } break;
                case '@': {
                    lab_row[j] = true;

                    start.x = j;
                    start.y = i;
                } break;
                case '#': {
                    lab_row[j] = true;

                    finish.x = j;
                    finish.y = i;
                } break;
                default: {
                    throw std::invalid_argument (row);
                }
            }
        }
    }

    return {lab, start, finish};
}

std::ostream&
print_lab (const std::vector <std::vector <bool>>& lab_map,
           coord_t start,
           coord_t finish,
           std::ostream& os = std::cout) {
    const auto size_y = lab_map.size ();
    const auto size_x = lab_map[0].size ();

    for (int y = 0; y < size_y; ++y) {
        for (int x = 0; x < size_x; ++x) {
            coord_t coord {x, y};
            if (coord == start) {
                os << '@';
            } else if (coord == finish) {
                os << '#';
            } else {
                os << (lab_map[y][x] ? '.' : '+');
            }
        }
        os << '\n';
    }

    return os;
}

std::size_t
solve (const std::vector <std::vector <bool>>& lab_map,
       coord_t start,
       coord_t finish) {


    return -1;
}