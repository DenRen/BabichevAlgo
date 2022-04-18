#include <iostream>
#include <vector>

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

enum class COLOR : char {
    WHITE, GREY, BLACK
};

void
DFS (const std::vector <std::vector <int>>& graph,
     std::size_t v_i,
     std::vector <COLOR>& color) {
    color[v_i] = COLOR::GREY;

    const auto& root = graph[v_i];
    for (std::size_t i = 0; i < root.size (); ++i) {
        auto neigh_i = root[i];
        if (color[neigh_i] == COLOR::WHITE) {
            DFS (graph, neigh_i, color);
        }
    }
    color[v_i] = COLOR::BLACK;
}

int
solve (const std::vector <std::vector <bool>>& area) {
    auto graph = build_graph (area);

    std::vector <COLOR> color (graph.size (), COLOR::WHITE);
    
    int num_conn_comp = 0;
    for (std::size_t i = 0; i < graph.size (); ++i) {
        if (color[i] == COLOR::WHITE) {
            DFS (graph, i, color);
            ++num_conn_comp;
        }
    }

    return num_conn_comp;
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
    }

    std::cout << solve (area) << '\n';
}
