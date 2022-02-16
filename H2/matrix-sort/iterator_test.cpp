#include <iostream>
#include <iterator>

#include "../../libs/other_func.hpp"

template <typename Iterator>
class counter_iterator {
    unsigned counter = 0;

protected:
    Iterator iter_;

public:
    counter_iterator (Iterator iter) :
        iter_ (iter)
    {}

    typename Iterator::reference
    operator * () noexcept {
        ++counter;
        return *iter_;
    }

    unsigned
    count () const noexcept {
        return counter;
    }

    const Iterator&
    base () const noexcept {
        return iter_;
    }

    Iterator&
    base () noexcept {
        return iter_;
    }

    Iterator&
    operator ++ () {
        return ++iter_;
    }
};

template <typename Iterator>
bool
operator == (const counter_iterator <Iterator>& lhs,
             const counter_iterator <Iterator>& rhs) {
    return lhs.base () == rhs.base ();
}

template <typename Iterator>
bool
operator != (const counter_iterator <Iterator>& lhs,
             const counter_iterator <Iterator>& rhs) {
    return lhs.base () != rhs.base ();
}

int main () {
    seclib::RandomGenerator rand;
    auto vec = rand.get_vector <int> (10, 10);

    counter_iterator iter_begin {vec.begin ()};
    counter_iterator iter_end {vec.end ()};
    for (; iter_begin != iter_end; ++iter_begin) {
        *iter_begin;
        *iter_begin;
    }

    std::cout << iter_begin.count () << std::endl;
}