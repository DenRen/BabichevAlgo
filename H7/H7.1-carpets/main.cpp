#include "solve.hpp"

void
add_neighbor (const std::vector <std::vector <bool>>& area,
              const std::vector <int>& id_node,
              std::vector <int>& node,
              int pos_i,
              int pos_j,
              int off_i,
              int off_j) {
    const long N = area.size (), M = area[0].size ();
    
    int i = pos_i + off_i, j = pos_j + off_j;

    if (i >= 0 && i < N) {
        if (j >= 0 && j < M) {
            if (area[i][j]) {
                auto it = std::lower_bound (id_node.cbegin (), id_node.cend (), i * N + j);
                int i_node = std::distance (id_node.cbegin (), it);
                node.push_back (i_node);
            }
        }
    }
}

std::vector <std::vector <int>>
build_graph (const std::vector <std::vector <bool>>& area) {
    const long N = area.size (), M = area[0].size ();

    std::vector <int> id_node;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (area[i][j]) {
                id_node.push_back (i * N + j);
            }
        }
    }

    std::vector <std::vector <int>> graph (id_node.size ());

    int num_graph = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (!area[i][j]) {
                continue;
            }

            auto& node = graph[num_graph++];

            add_neighbor (area, id_node, node, i, j,  1,  0);
            add_neighbor (area, id_node, node, i, j, -1,  0);
            add_neighbor (area, id_node, node, i, j,  0,  1);
            add_neighbor (area, id_node, node, i, j,  0, -1);
        }
    }

    return graph;
}

int
solve (const std::vector <std::vector <bool>>& area) {
    auto graph = build_graph (area);

    

    return -1;
}

int main () {
    std::ios_base::sync_with_stdio (false);

    int N = 0, M = 0;
    std::cin >> N >> M;
    
    std::vector <std::vector <bool>> area (N);
    std::string str;
    for (auto& row : area) {
        std::cin >> str;
        row.resize (M);
        for (int i = 0; i < M; ++i) {
            row[i] = str[i] == '+';
        }

        DUMP (row);
    }

    std::cout << solve (area) << '\n';
}
