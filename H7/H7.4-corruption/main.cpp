#include <vector>
#include <iostream>
#include <set>

struct edge_t {
    unsigned to, weight;
    
    edge_t (unsigned to = 0, unsigned weight = 0) :
        to (to), weight (weight)
    {}

    bool
    operator < (const edge_t& rhs) const noexcept {
        return weight > rhs.weight ||
               (weight == rhs.weight && to < rhs.to);
    }
};

std::ostream&
operator << (std::ostream& os, const edge_t& edge) {
    return os << '{' << edge.to << ": " << edge.weight << '}';
}

using node_t = std::vector <edge_t>;
using graph_t = std::vector <node_t>;

struct solver_t {
    graph_t graph;

    template <typename T>
    solver_t (T&& graph) :
        graph (std::forward <T> (graph))
    {}

    int
    solve () {
        const auto size = graph.size ();

        std::vector <unsigned> v_min_dists (size, -1);
        v_min_dists[0] = 0;
        
        std::set <edge_t> edges_nmst;
        for (const auto&edge : graph[0]) {
            edges_nmst.insert (edge);
            v_min_dists[edge.to] = 0;
        }
        
        std::vector <bool> in_mst (size, false);
        in_mst[0] = true;

        while (edges_nmst.size ()) {
            auto min_elem_it = edges_nmst.cbegin ();
            int new_mst_id = min_elem_it->to;
            edges_nmst.erase (min_elem_it);

            in_mst[new_mst_id] = true;
            const auto& min_elem = graph[new_mst_id]; 
            for (const auto&[to, weight] : min_elem) {
                if (in_mst[to]) {
                    continue;
                }

                // to in nmst
                const auto& node = graph[to];
                auto& p_id = v_min_dists[to];
                if (p_id == -1u) {
                    edges_nmst.emplace (to, weight);
                    p_id = new_mst_id;
                } else {
                    edge_t p_edge;
                    for (const auto& edge : node) {
                        if (edge.to == p_id) {
                            p_edge = edge;
                            break;
                        }
                    }
                    
                    if (p_edge.weight < weight) {
                        p_id = new_mst_id;
                        
                        edges_nmst.erase ({to, p_edge.weight});
                        edges_nmst.emplace (to, weight);
                    }
                }
            }
        }
        
        int full_len = 0;
        for (std::size_t i = 1; i < size; ++i) {
            for (const auto& edge : graph[i]) {
                if (edge.to == v_min_dists[i]) {
                    full_len += edge.weight;
                    break;
                }
            }
        }

        return full_len;
    }
};

int main () {
    std::ios_base::sync_with_stdio (false);
    
    int N = 0;
    std::cin >> N;

    graph_t graph (N);
    for (auto& v : graph) {
        for (int i = 0; i < N; ++i) {
            int weight = 0;
            std::cin >> weight;
            
            if (weight) {
                v.emplace_back (i, weight);
            }
        }
    }

    solver_t solver {std::move (graph)};
    std::cout << solver.solve () << '\n';
}
