#include "solve.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

void
fill_str (const std::vector <std::vector <int>>& graph,
          int begin_i,
          std::string& str,
          std::set <int>& set) {
    if (set.find (begin_i) != set.cend ()) {
        return;
    }

    set.insert (begin_i);

    const auto& row = graph[begin_i];
    for (int i : row) {
        str[i] = '1';
        fill_str (graph, i, str, set);
    }
}

int main () {
    std::ios_base::sync_with_stdio (false);

    int N = 0;
    std::cin >> N;

    std::vector <std::vector <int>> graph (N);

    std::string str;
    for (int i = 0; i < N; ++i) {
        auto& row = graph[i];

        std::cin >> str;
        for (auto j = str.find ('1'); j != std::string::npos;
                  j = str.find ('1', j + 1)) {
            row.push_back (j);
        }
    }
    
    for (const auto& r : graph) {
        DUMP (r);
    }

    str.resize (N);
    std::set <int> set;
    for (int i = 0; i < N; ++i) {
        std::fill_n (str.begin (), N, '0');
        set.clear ();

        fill_str (graph, i, str, set);
        std::cout << str << '\n';
    }
}
