#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <algorithm>

#include "../print_lib.hpp"

template <typename T>
struct my_time_t {
    T hour, minute;

    my_time_t () :
        hour (0),
        minute (0)
    {}

    T to_minutes () const noexcept {
        return 60 * hour + minute;
    }
};

template <typename T>
std::istream&
operator >> (std::istream& is, my_time_t <T>& time) {
    char c;
    return is >> time.hour >> c >> time.minute;
}

template <typename T>
std::ostream&
operator << (std::ostream& os, const my_time_t <T>& time) {
    return os << std::setfill ('0') << std::setw (2) << time.hour << ":"
              << std::setfill ('0') << std::setw (2) << time.minute;
}

template <typename T>
struct interval_t {
    T begin, end;

    interval_t (T begin, T end) :
        begin (begin),
        end (end)
    {}

    T len () const noexcept {
        return end - begin;
    }
};

template <typename T>
std::ostream&
operator << (std::ostream& os, const interval_t <T>& interval) {
    return os << "[" << interval.begin << ", " << interval.end << "]";
}

template <typename T, typename RetT = std::size_t>
std::vector <RetT>
calc_interval_coverage (const std::vector <interval_t <T>>& ints) {
    std::vector <RetT> cov (ints.size ());



    return cov;
}

template <typename T>
std::size_t
busy_cells (const std::vector <std::pair <my_time_t <T>, my_time_t <T>>>& time_ints) {
    // Convert time intervals to minutes points of begin and end
    std::vector <T> begins, ends;
    begins.reserve (time_ints.size ());
    ends.reserve (time_ints.size ());

    for (const auto& time_int : time_ints) {
        begins.push_back (time_int.first.to_minutes ());
        ends.push_back (time_int.second.to_minutes ());
    }

    // Sort begins and ends
    std::sort (begins.begin (), begins.end ());
    std::sort (ends.begin (), ends.end ());
    
    // Find max coverage
    T cov = 1, res = cov;
    for (auto begin = begins.cbegin () + 1, end = ends.cbegin ();
        begin != begins.cend (); ) {
        if (*begin <= *end) {
            ++begin;
            ++cov;
        } else {
            ++end;
            --cov;
        }

        if (cov > res) {
            res = cov;
        }
    }

    return res;
}