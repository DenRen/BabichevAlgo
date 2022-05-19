#pragma once

#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include "../../libs/print_lib.hpp"

template <typename T>
bool operator != (const std::vector <T>& lhs,
                  const std::vector <T>& rhs)
{
    const std::size_t size = lhs.size ();
    if (size != rhs.size ()) {
        return true;
    }

    for (std::size_t i = 0; i < size; ++i) {
        if (lhs[i] != rhs[i]) {
            return true;
        }
    }

    return false;
} // bool operator != (const std::vector <T>& lhs, const std::vector <T>& rhs)

template <typename T>
bool operator == (const std::vector <T>& lhs,
                  const std::vector <T>& rhs)
{
    const std::size_t size = lhs.size ();
    if (size != rhs.size ()) {
        return false;
    }

    for (std::size_t i = 0; i < size; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }

    return true;
} // bool operator == (const std::vector <T>& lhs, const std::vector <T>& rhs)

template <typename T, typename Rand>
std::vector <T>
getRandFillVector (std::size_t size,
                   Rand& rand)
{
    std::vector <T> vec (size);
    for (auto& item : vec) {
        item = rand ();
    }

    return vec; // RVO
} // getRandFillVector (std::size_t size, Rand& rand)

template <typename T, typename Rand>
std::vector <T>
getUniqRandFillVector (std::size_t size,
                       Rand& rand)
{
    std::set <T> set;
    while (set.size () != size) {
        set.insert (rand ());
    }

    std::vector <T> res;
    res.reserve (size);
    for (const auto& item : set) {
        res.push_back (item);
    }

    return res; // RVO
} // getRandFillVector (std::size_t size, Rand& rand)

template <typename T, typename Rand>
std::vector <T>
getUniqRandFillVector (std::size_t size,
                       Rand& rand,
                       T module)
{
    std::set <T> set;
    while (set.size () != size) {
        set.insert (rand () % module);
    }

    std::vector <T> res;
    res.reserve (size);
    for (const auto& item : set) {
        res.push_back (item);
    }

    return res; // RVO
} // getRandFillVector (std::size_t size, Rand& rand)s

template <typename T, typename Rand>
std::vector <T>
getRandFillVector (std::size_t size,
                   Rand& rand,
                   T module)
{
    std::vector <T> vec (size);
    for (auto& item : vec) {
        item = rand () % module;
    }

    return vec; // RVO
} // getRandFillVector (std::size_t size, Rand& rand, T module)