#pragma once

#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include "other_func.hpp"

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

// ----------------------------------------------------------------------------

template <typename T>
std::ostream&
operator << (std::ostream& os,
             const std::vector <T>& vec) {
    const std::size_t size = vec.size ();

    if (size == 0) {
        return os;
    }

    for (std::size_t i = 0; i + 1 < size; ++i) {
        os << vec[i] << " ";
    }

    return os << vec[size - 1];
}

template <typename K, typename V>
std::ostream&
operator << (std::ostream& os,
             const std::map <K, V>& map) {
    os << "{";
    auto i = map.size ();
    for (const auto& [key, value] : map) {
        os << key << ": " << value;
        if (--i > 0) {
            os << ", ";
        }
    }

    return os << "}";
}

template <typename... Args>
std::ostream&
print_all (std::ostream& os,
           const Args&... args) {
    return (os << ... << args);
}

template <typename SepT, typename Arg, typename... Args>
std::ostream&
print_all_sep (std::ostream& os,
               const Arg& arg,
               const Args&... args) {
    return ((os << arg) << ... << print_all (args, " "));
}

template <typename... Args>
std::ostream&
operator << (std::ostream& os,
             const std::tuple <Args...>& items) {
    print_all_sep (os, (std::get <Args> (items))...);
    return os;
}

// ----------------------------------------------------------------------------

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