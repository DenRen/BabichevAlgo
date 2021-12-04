#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

template <typename T>
std::istream& operator >> (std::istream& is, std::vector <T>& vec) {
    T value;

    is >> value;
    while (value != 0) {
        vec.push_back (value);
        is >> value;
    }

    return is;
}

template <typename T>
std::vector <T> solve (std::vector <T>& a, std::vector <T>& b) {
    std::sort (a.begin (), a.end ());
    std::sort (b.begin (), b.end ());

    if (a.size () == 0) {
        return b;
    } else if (b.size () == 0) {
        return a;
    }

    // Here a.size () > 0 && b.size () > 0
    std::vector <T> res;
    res.reserve (a.size () + b.size ());


    auto iter_a = a.cbegin (), iter_b = b.cbegin ();
    while (iter_a != a.cend () && iter_b != b.cend ()) {
        if (*iter_a < *iter_b) {
            res.push_back (*iter_a++);
        } else if (*iter_a == *iter_b) {
            ++iter_a;
            ++iter_b;
        } else {
            res.push_back (*iter_b++);
        }
    }

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