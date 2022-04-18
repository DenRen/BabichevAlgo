#include <iostream>
#include <vector>
#include <algorithm>

struct coord_t {
    int x, y;
    
    coord_t (int x, int y) :
        x (x), y (y)
    {}
};

struct solver_t {
    const int num_V;
    std::vector <std::vector <int>> graph;
    std::vector <std::pair <int, int>> block_edges;    
    
    std::vector <int> colors;

    template <typename G, typename Be>
    solver_t (G&& graph, Be block_edges) :
        num_V (graph.size ()),
        graph (std::forward <G> (graph)),
        block_edges (std::forward <Be> (block_edges)),
        colors (num_V, 0)
    {}

    void
    remove_edge (int v, int w) {
        graph[v].erase (std::remove (graph[v].begin (), graph[v].end (), w), graph[v].end ());
        graph[w].erase (std::remove (graph[w].begin (), graph[w].end (), v), graph[w].end ());
    }
    
    void
    paint_conn_comp (int i, int new_color) {
        colors[i] = new_color;

        auto& node = graph[i];
        for (const auto& neigh_i : node) {
            if (colors[neigh_i] != new_color) {
                paint_conn_comp (neigh_i, new_color);
            }
        }
    }

    // false - num conn comp does not change
    // true - else
    bool
    add_edge (int v_i, int w_i) {
        auto& v = graph[v_i], &w = graph[w_i];
        
        bool is_diff_cc = colors[v_i] != colors[w_i];
        if (is_diff_cc) {
           paint_conn_comp (w_i, colors[v_i]); 
        }

        v.push_back (w_i);
        w.push_back (v_i);

        return is_diff_cc;
    }

    void
    mark_conn_comp (int i, int color) {
        colors[i] = color;

        const auto& node = graph[i];
        for (const auto& neigh_i : node) {
            if (colors[neigh_i]) {
                continue;
            }

            mark_conn_comp (neigh_i, color);    
        }
    }

    int
    mark_conn_comp () {
        int num_conn_comp = 0;
    
        int cur_color = 1;
        for (int i = 0; i < num_V; ++i) {
            if (colors[i]) {
                continue;
            }

            mark_conn_comp (i, cur_color++); 
            ++num_conn_comp;
        }

        return num_conn_comp;
    }

    std::vector <int>
    solve () {
        std::vector <int> fortified_area (block_edges.size () + 1);
        
        for (const auto&[v, w] : block_edges) {
            remove_edge (v, w);
        }
        
        int ff_edge_i = block_edges.size ();
        fortified_area[ff_edge_i--] = mark_conn_comp ();
        
        while (ff_edge_i + 1) {
            const auto&[v_i, w_i] = block_edges[ff_edge_i];

            fortified_area[ff_edge_i] = fortified_area[ff_edge_i + 1] - add_edge (v_i, w_i);
            --ff_edge_i;
        }

        return fortified_area;
    }

    void
    dump_graph () const {
        for (std::size_t i = 0; i < graph.size (); ++i) {
            std::cout << "c: " << colors[i] << ", ";
//            DUMP (graph[i]);
        }
        std::cout << '\n';
    }
};

int main () {
    std::ios_base::sync_with_stdio (false);
    
    int V = 0, E = 0, M = 0;
    std::cin >> V >> E >> M;

    std::vector <std::vector <int>> graph (V);
    for (int i = 0; i < E; ++i) {
        int v = 0, w = 0;
        std::cin >> v >> w;
        
        graph[v].push_back (w);
        graph[w].push_back (v);
    }

    std::vector <std::pair <int, int>> block_edges (M);
    for (auto&[lhs, rhs] : block_edges) {
        std::cin >> lhs >> rhs;
    }

    solver_t solver {std::move (graph), std::move (block_edges)};
    auto res = solver.solve ();
    
    for (const auto& item : res) {
        std::cout << item << '\n';
    }
}
