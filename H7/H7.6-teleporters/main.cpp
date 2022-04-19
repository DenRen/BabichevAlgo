#include "solve.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

// Comp force conn
struct node_cfc_t {
    std::vector <int> v;
    std::set <int> neigh;
};

std::ostream&
operator << (std::ostream& os, const node_cfc_t& node) {
    return os << "{[" << node.v << "] -> {" << node.neigh << "}}";
}

void
DFS_first (const std::vector <std::vector <int>>& g,
           std::vector <int>& times,
           int i,
           int& time) {
    times[i] = -2;

    ++time;

    for (const auto& neigh_i : g[i]) {
        if (times[neigh_i] == -1) {
            DFS_first (g, times, neigh_i, time);
        }
    }

    times[i] = time++;
}

void
DFS_first (const std::vector <std::vector <int>>& g,
           std::vector <int>& times) {
    int time = -1;

    for (std::size_t i = 0; i < g.size (); ++i) {
        if (times[i] == -1) {
            DFS_first (g, times, i, time);
        }
    }
}

struct time_node_t {
    int time, id;

    time_node_t (int time = 0, int id = 0) :
        time (time), id (id)
    {}

    bool
    operator < (const time_node_t& rhs) const noexcept {
        return time < rhs.time;
    }
};

void
DFS_second (const std::vector <std::vector <int>>& gT,
            int begin_i,
            node_cfc_t& node_cfc,
            std::vector <int>& times,
            std::set <time_node_t>& time_set) {
    time_set.erase ({times[begin_i], begin_i});
    times[begin_i] = -1;
    
    node_cfc.v.push_back (begin_i);
    for (int neigh_i : gT[begin_i]) {
        if (times[neigh_i] != -1) {
            DFS_second (gT, neigh_i, node_cfc, times, time_set);
        }
    }
}

std::pair <std::vector <node_cfc_t>, std::vector <int>>
DFS_second (const std::vector <std::vector <int>>& gT,
            std::vector <int>& times) {
    const long size = gT.size ();
    std::vector <node_cfc_t> cfc_graph;
    
    std::set <time_node_t> time_set;
    for (int i = 0; i < size; ++i) {
        time_set.emplace (times[i], i);
    }
    
    std::vector <int> num_cfc (size);
    while (time_set.size ()) {
        auto it = time_set.end ();
        --it;

        node_cfc_t node_cfc;
        DFS_second (gT, it->id, node_cfc, times, time_set);
        
        int cur_num_cfc = cfc_graph.size ();
        for (int i : node_cfc.v) {
            num_cfc[i] = cur_num_cfc;
        }

        cfc_graph.push_back (std::move (node_cfc));
    }

    return {cfc_graph, num_cfc};
}

void
add_neigh (std::vector <node_cfc_t>& cfc_g,
           const std::vector <int>& num_cfc,
           const std::vector <std::vector <int>>& g) {
    int cur_cfc_num = 0;
    for (auto& cfc : cfc_g) {
        for (const auto& v_i : cfc.v) {
            for (const auto& neigh_i : g[v_i]) {
                if (num_cfc[neigh_i] != cur_cfc_num) {
                    cfc.neigh.insert (num_cfc[neigh_i]);
                }
            }
        }
        
        ++cur_cfc_num;
    } 
}

std::vector <node_cfc_t>
build_cfc_graph (const std::vector <std::vector <int>>& g,
                 const std::vector <std::vector <int>>& gT) {
    const long N = g.size ();

    std::vector <int> times (N, -1);
    DFS_first (g, times);
    auto[cfc_graph, num_cfc] = DFS_second (gT, times); 
    add_neigh (cfc_graph, num_cfc, g);

    return cfc_graph;
}

void
DFS_build_bit_map (std::vector <std::vector <bool>>& bitmap,
                   const std::vector <node_cfc_t>& cfc_g,
                   int cfc_i,
                   std::vector <bool>& is_cfc_painted) {
    const auto& cfc_node = cfc_g[cfc_i];
    const auto& v_arr = cfc_node.v;

    DUMP (cfc_i);

    int begin_v_i = v_arr[0];
    auto& bitrow = bitmap[begin_v_i];
    const long N = bitmap.size ();

    for (const auto& neigh_cfc_i : cfc_node.neigh) {
        if (!is_cfc_painted[neigh_cfc_i]) {
            DFS_build_bit_map (bitmap, cfc_g, neigh_cfc_i, is_cfc_painted);
        }
        
        const auto node_neigh = cfc_g[neigh_cfc_i];
        
        int begin_other_v_i = node_neigh.v[0];
        const auto& other_bitrow = bitmap[begin_other_v_i];
        if (node_neigh.neigh.size ()) {
            for (int i = 0; i < N; ++i) {
                bitrow[i] = bitrow[i] | other_bitrow [i];
            }
        }
        
        bitrow[begin_other_v_i] = true;
    }

    if (v_arr.size () > 1) {
        for (const auto& v_i : v_arr) {
            bitrow[v_i] = true;
        }
    }
    
    for (auto it = ++v_arr.cbegin (); it != v_arr.cend (); ++it) {
        bitmap[*it] = bitrow;
    }

    is_cfc_painted[cfc_i] = true;

    std::cout << "end: " << cfc_i << '\n';
}

std::vector <std::vector <bool>>
build_bit_map (const std::vector <node_cfc_t>& cfc_g,
               int num_vertex) {
    std::vector <std::vector <bool>> bitmap (num_vertex);
    for (auto& bitrow : bitmap) {
        bitrow = std::vector <bool> (num_vertex, false);
    }
    
    std::vector <bool> is_cfc_painted (cfc_g.size (), false);
    for (std::size_t i = 0; i < cfc_g.size (); ++i) {
        if (is_cfc_painted[i]) {
            continue;
        }

        DFS_build_bit_map (bitmap, cfc_g, i, is_cfc_painted);
    }

    return bitmap;
}

int main () {
    std::ios_base::sync_with_stdio (false);

    int N = 0;
    std::cin >> N;

    std::vector <std::vector <int>> graph (N), graph_T (N);

    std::string str;
    for (int i = 0; i < N; ++i) {
        auto& row = graph[i];

        std::cin >> str;
        for (auto j = str.find ('1'); j != std::string::npos;
                  j = str.find ('1', j + 1)) {
            row.push_back (j);
            graph_T[j].push_back (i);
        }
    }
   
    auto cfc_graph = build_cfc_graph (graph, graph_T);
    DUMP (cfc_graph);
    auto bit_map = build_bit_map (cfc_graph, N); 
    
    for (const auto& row : bit_map) {
        DUMP (row);
    }

/*    for (const auto& r : graph) {
        DUMP (r);
    }

    for (const auto& rT : graph_T) {
        DUMP (rT);
    }*/
}
