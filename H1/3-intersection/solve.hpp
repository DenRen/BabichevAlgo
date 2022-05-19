#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

// a.size () >= 2 && b.size () >= 2
template <typename T>
std::vector <T> solve (std::vector <T>& a, std::vector <T>& b) {
    std::vector <T> res;
    res.reserve (a.size () + b.size ());

    auto iter_a = a.cbegin (), iter_b = b.cbegin ();
    while (iter_a != a.cend () && iter_b != b.cend ()) {
        if (*iter_a < *iter_b) {
            ++iter_a;
        } else if (*iter_a == *iter_b) {
            res.push_back (*iter_a);
            ++iter_a;
            ++iter_b;
        } else {
            ++iter_b;
        }
    }

    return res;

    if (iter_a == a.cend () && iter_b == b.cend ()) {
        return res;
    }

    if (iter_a != a.cend ()) {
        do {
            res.push_back (*iter_a++);
        } while (iter_a != a.cend ());
    } else {
        do {
            res.push_back (*iter_b++);
        } while (iter_b != b.cend ());
    }

    return res;
}