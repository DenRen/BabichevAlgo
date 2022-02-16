#include <vector>
#include <random>
#include <iterator>

#include "../../libs/other_func.hpp"

template <typename FirstIterator, typename SecondIterator>
class dominant_first_pair_iterator {
    FirstIterator iter_first_;
    SecondIterator iter_second_;

public:
    using value_type = typename 
    using difference_type = typename FirstIterator::difference_type;
    using first_reference = typename FirstIterator::reference;
    using second_reference = typename SecondIterator::reference;

    dominant_first_pair_iterator (FirstIterator iter_first,
                                  SecondIterator iter_second) noexcept :
        iter_first_ (iter_first),
        iter_second_ (iter_second)
    {}

    first_reference
    operator * () noexcept
    {
        return *iter_first_;
    }
    
    std::pair <const first_reference, const second_reference>
    operator * () const noexcept
    {
        return {*iter_first_, *iter_second_};
    }

    dominant_first_pair_iterator&
    operator ++ () noexcept
    {
        ++iter_first_;
        ++iter_second_;
        return *this;
    }

    dominant_first_pair_iterator&
    operator += (difference_type n) noexcept {
        iter_first_ += n;
        iter_second_ += n;
        return *this;
    }

    dominant_first_pair_iterator&
    operator -= (difference_type n) noexcept {
        return *this += (-n);
    }

    dominant_first_pair_iterator
    operator ++ (int) noexcept
    {
        return dominant_first_pair_iterator {iter_first_++, iter_second_++};
    }

    std::pair <FirstIterator, SecondIterator>
    base () const noexcept
    {
        return {iter_first_, iter_second_};
    }
};

template <typename FirstIterator, typename SecondIterator>
typename dominant_first_pair_iterator <FirstIterator, SecondIterator>::difference_type
operator - (const dominant_first_pair_iterator <FirstIterator, SecondIterator>& lhs,
            const dominant_first_pair_iterator <FirstIterator, SecondIterator>& rhs) noexcept
{
    return std::distance (lhs.base ().first, rhs.base ().first);
}

template <typename FirstIterator, typename SecondIterator>
dominant_first_pair_iterator <FirstIterator, SecondIterator>
operator + (const dominant_first_pair_iterator <FirstIterator, SecondIterator>& lhs,
            typename dominant_first_pair_iterator
                     <FirstIterator, SecondIterator>::difference_type value) noexcept
{
    auto iter = lhs;
    return iter += value;
}

template <typename FirstIterator, typename SecondIterator>
dominant_first_pair_iterator <FirstIterator, SecondIterator>
operator - (const dominant_first_pair_iterator <FirstIterator, SecondIterator>& lhs,
            typename dominant_first_pair_iterator
                     <FirstIterator, SecondIterator>::difference_type value) noexcept
{
    auto iter = lhs;
    return iter -= value;
}

template <typename FirstIterator, typename SecondIterator>
bool
operator == (const dominant_first_pair_iterator <FirstIterator, SecondIterator>& lhs,
             const dominant_first_pair_iterator <FirstIterator, SecondIterator>& rhs) noexcept
{
    return lhs.base ().first == rhs.base ().first;
}

template <typename FirstIterator, typename SecondIterator>
bool
operator != (const dominant_first_pair_iterator <FirstIterator, SecondIterator>& lhs,
             const dominant_first_pair_iterator <FirstIterator, SecondIterator>& rhs) noexcept
{
    return !(lhs == rhs);
}

template <typename FirstIterator, typename SecondIterator>
bool
operator < (const dominant_first_pair_iterator <FirstIterator, SecondIterator>& lhs,
            const dominant_first_pair_iterator <FirstIterator, SecondIterator>& rhs)
{
    return lhs.base ().first < lhs.base ().first;
}

template <typename PosT, typename T>
std::vector <PosT>
get_sort_sequence (const std::vector <T>& shuffled)
{
    const auto size = shuffled.size ();

    std::vector <PosT> pos;
    pos.reserve (size);
    for (std::size_t i = 0; i < size; ++i) {
        pos.push_back (i);
    }

    std::vector <T> data = shuffled;
    dominant_first_pair_iterator begin {data.begin (), pos.begin ()};
    dominant_first_pair_iterator end {data.end (), pos.end ()};

    std::sort (begin, end, [] (const auto& lhs, const auto& rhs) {
        // return lhs.first < rhs.first;
        return true;
        
    });

    return pos;
}

int main () {
    seclib::RandomGenerator rgen;

    auto shuffled = rgen.get_vector <int> (10, 10);
    std::cout << shuffled << std::endl;

    auto sort_seq = get_sort_sequence <std::size_t> (shuffled);
    std::cout << sort_seq << std::endl;
}