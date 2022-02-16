#include <gtest/gtest.h>
#include <random>
#include <sstream>

#include "other_func.hpp"
#include "solve.hpp"

// g++ test.cpp -lgtest -lgtest_main -lpthread -Werror -std=c++17 -O3 && clear && ./a.out

using std::cout;
using std::endl;

template <typename F>
void all (F&& func,
          std::vector <bool>& vec,
          int deep = 0)
{
    if (deep == vec.size ()) {
        cout << vec << endl;
        return;
    }

    vec[deep] = false;
    all (func, vec, deep + 1);

    vec[deep] = true;
    all (func, vec, deep + 1);
}

bool is_conn_vec (const std::vector <bool>& vec)
{
    if (vec.size () == 0) {
        throw std::invalid_argument ("Vector size is zero!");
    }
    
    if ((vec[0] && vec[vec.size () - 1]) == false) {
        return false;
    }

    for (std::size_t i = 1; i + 1 < vec.size (); ++i) {
        if ((vec[i] || vec[i+1]) == false) {
            return false;
        }
    }

    return true;
}

int full_len (const std::vector <int>& points,
              const std::vector <bool>& bitmap)
{
    if (points.size () <= 1) {
        throw std::invalid_argument ("Error vector points");
    } else if (points.size () == 2) {
        return points[1] - points[0];
    }

    const auto size = points.size ();
    int len = 0;

    for (int i = 1; i < size; ++i) {
        if (bitmap[i-1]) {
            len += points[i] - points[i - 1];
        }
    }

    return len;
}

TEST (FIND_BUG, _) {
    std::random_device rd;
    std::mt19937 mersenne {rd ()};

    std::size_t size = 10;
    std::vector <int> rand_vec = getUniqRandFillVector <int> (size, 40, mersenne);

    std::vector <bool> bitmap (size - 1, false);
    // all ([]{}, bitmap);

    cout << full_len ({0, 3, 4, 5, 7}, {1, 0, 1, 1}) << endl;

    // Сделать перебор всех валидных путей
}