#include "solve.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

// Comp force conn
struct node_cfc_t {
    std::vector <int> neigh;
    std::vector <int> v;
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

std::vector <node_cfc_t>
DFS_second (const std::vector <std::vector <int>>& gT,
            std::vector <int>& times) {
    const long size = gT.size ();
    std::vector <node_cfc_t> cfc_graph;
    
    std::set <time_node_t> time_set;
    for (int i = 0; i < size; ++i) {
        time_set.emplace (times[i], i);
    }

    while (time_set.size ()) {
        auto it = time_set.end ();
        --it;

        node_cfc_t node_cfc;
        DFS_second (gT, it->id, node_cfc, times, time_set);
        cfc_graph.push_back (std::move (node_cfc));
    }

    return cfc_graph;
}

std::vector <node_cfc_t>
build_cfc_graph (const std::vector <std::vector <int>>& g,
                 const std::vector <std::vector <int>>& gT) {
    const long N = g.size ();

    std::vector <int> times (N, -1);
    DFS_first (g, times);
    auto cfc_graph = DFS_second (gT, times);
    DUMP (cfc_graph);
    return {};
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

/*    for (const auto& r : graph) {
        DUMP (r);
    }

    for (const auto& rT : graph_T) {
        DUMP (rT);
    }*/
}
