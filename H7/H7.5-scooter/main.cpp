//#include "solve.hpp"

#include <iostream>
#include <vector>
#include <set>

struct edge_t {
    int to;
    unsigned weight;

    edge_t (int to = 0, unsigned weight = 0) :
        to (to), weight (weight)
    {}

    bool
    operator < (const edge_t& rhs) const noexcept {
        return weight < rhs.weight ||
               (weight == rhs.weight && to < rhs.to);
    }
};

std::ostream&
operator << (std::ostream& os, const edge_t& e) {
    return os << '{' << e.to << ": " << e.weight << '}';
}

using node_t = std::vector <edge_t>;
using graph_t = std::vector <node_t>;

// todo: optimize
graph_t
h_map2graph (const std::vector <std::vector <unsigned>>& h_map) {
    const int N = h_map.size (), M = h_map[0].size ();
    graph_t graph (N * M);

    auto add_node = [&] (int x, int y, int off_x, int off_y) {
        auto& node = graph[x + y * M];
        unsigned h = h_map[y][x];
        
        x += off_x;
        y += off_y;
        
        if (y >= 0 && y < N && x >= 0 && x < M) {
            unsigned other_h = h_map[y][x];
            unsigned delta_h = other_h <= h ? 0 : other_h - h;

            node.push_back ({x + y * M, delta_h});
        }
    };

    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            add_node (x, y,  1,  0);
            add_node (x, y, -1,  0);
            add_node (x, y,  0,  1);
            add_node (x, y,  0, -1);
        }
    }

    return graph;
}

void
dump_graph (const graph_t& graph) {
/*    for (const auto& r : graph) {
        DUMP (r);
    }*/
}

void
dump_d (const std::vector <unsigned>& d,
        unsigned x_size) {
    const auto y_size = d.size () / x_size;
    for (unsigned i = 0; i < y_size; ++i) {
        for (unsigned j = 0; j < x_size; ++j) {
            std::cout << d[j + i * x_size] << ' ';
        }
        std::cout << '\n';
    }
}

unsigned
get_weight (const graph_t& graph, int v_i, int w_i) {
    const auto& node = graph[v_i];
    for (unsigned i = 0; i < node.size (); ++i) {
        if (node[i].to == w_i) {
            return node[i].weight;
        }
    }

    return -1u;
}

std::vector <unsigned>
dijkstra (const graph_t& graph,
          int start,
          int finish) {
    const long size = graph.size ();
    
    std::vector <unsigned> d (size, -1u);
    std::vector <bool> in_spt (size, false);
    d[start] = 0;
    in_spt[start] = true;
    
    std::set <edge_t> edges_nspt;
    for (const auto& edge : graph[start]) {
        edges_nspt.insert (edge);
        d[edge.to] = edge.weight;
    }

    while (edges_nspt.size ()) {
        auto min_elem_it = edges_nspt.cbegin ();
        edge_t min_elem = *min_elem_it;
        edges_nspt.erase (min_elem_it);

        in_spt[min_elem.to] = true;
        if (min_elem.to == finish) {
            return d;
        }

        const auto& neighs = graph[min_elem.to];
        for (const auto&[to, weight] : neighs) {
            if (in_spt[to]) {
                continue;
            }

            auto new_dist = d[min_elem.to] + weight;
            
            if (d[to] == -1u) {
                d[to] = new_dist;
                edges_nspt.emplace (to, new_dist);
            } else {
                if (d[to] > new_dist) {
                    edge_t old_edge {to, d[to]};
                    edges_nspt.erase (old_edge);

                    d[to] = new_dist;
                    edges_nspt.emplace (to, new_dist);
                }
            }
        }
    }
    
    return d;
}

int
solve (std::vector <std::vector <unsigned>>& h_map) {
    auto graph = h_map2graph (h_map);
    
    auto d = dijkstra (graph, 0, graph.size () - 1);
    
    return d[d.size () - 1];
}

int main () {
    std::ios_base::sync_with_stdio (false);

    unsigned N = 0, M = 0;
    std::cin >> N >> M;
    
    std::vector <std::vector <unsigned>> h_map (N);
    for (auto& h_row : h_map) {
        h_row.resize (M);
        for (auto& h : h_row) {
            std::cin >> h;
        }
    }

    std::cout << solve (h_map) << '\n';
}
