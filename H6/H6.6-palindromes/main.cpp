#include <iostream>
#include <vector>

class poli_solver {
    std::string str;
    std::vector <std::vector <std::size_t>> cache;

public:
    template <typename T>
    poli_solver (T&& str) :
        str (std::forward <T> (str)),
        cache (str.size ())
    {
        for (auto& row : cache) {
            row = std::vector <std::size_t> (str.size (), -1);
        }
    }

    std::size_t
    get_num_poli (int i_left, int i_right) {
        if (i_left < 0 || i_right >= str.size ()) {
            return 0;
        }
        
        auto& cache_item = cache[i_left][i_right];
        if (cache_item != -1) {
            return cache_item;
        }
        
        std::size_t num_poli = 0;
        for (int i = i_left; i >= 0; --i) {
            for (int j = i_right; j < str.size (); ++j) {
                if (str[i] == str[j]) {
                    num_poli += 1 + get_num_poli (i - 1, j + 1);
                }
            }
        }

        return cache_item = num_poli;
    }

    std::size_t
    solve () {
        std::size_t num_poli = str.size ();
        for (int i = 0; i < str.size () - 1; ++i) {
            num_poli += get_num_poli (i, i + 1);
        }

        return num_poli;
    }
};


int main () {
    std::ios_base::sync_with_stdio (false);

    std::string str;
    std::cin >> str;
    poli_solver solver {str};

    std::cout << solver.solve () << '\n';
}
