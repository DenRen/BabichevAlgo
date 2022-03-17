#include <iostream>
#include "solve.hpp"

// g++ tree_test.cpp && ./a.out && dot -Tpng graph.gv -o graph.png

int main () {
    btree_num_t tree;
    tree.root.keys[0] = 8;
    tree.root.keys[1] = 16;
    tree.root.size = 2;
    
    {
        tree.root.poss[0] = new btree_num_t::node_t;
        auto& cur = *tree.root.poss[0];
        cur.size = 3;
        cur.keys[0] = 0;
        cur.keys[1] = 5;
        cur.keys[2] = 7;
    }

    {
        tree.root.poss[1] = new btree_num_t::node_t;
        auto& cur = *tree.root.poss[1];
        cur.size = 2;
        cur.keys[0] = 13;
        cur.keys[1] = 15;
    }

    {
        tree.root.poss[2] = new btree_num_t::node_t;
        auto& cur = *tree.root.poss[2];
        cur.size = 1;
        cur.keys[0] = 23;
    }

    {
        tree.root.poss[2]->poss[1] = new btree_num_t::node_t;
        auto& cur = *tree.root.poss[2]->poss[1];
        cur.size = 1;
        cur.keys[0] = -789;
    }

    tree.draw ();
}