#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <set>

// #include "../../libs/print_lib.hpp"

template <typename T>
std::ostream&
operator << (std::ostream& os,
             const std::vector <T>& vec)
{
    const std::size_t size = vec.size ();
    if (size == 0) {
        return os;
    }

    for (std::size_t i = 0; i + 1 < size; ++i) {
        os << vec[i] << " ";
    }

    return os << vec[size - 1];
}

template <typename T>
std::vector <std::vector <T>>
get_matrix (const std::size_t N,
            const std::size_t M,
            std::istream& is = std::cin)
{
    std::vector <std::vector <T>> matrix (N);
    for (auto& row : matrix) {
        std::istream_iterator <T> it_cin {is};
        row.reserve (M);
        std::copy_n (it_cin, M, std::back_inserter (row));
    }

    return matrix;
}

// lhs.size () == rhs.size ()
template <typename T>
bool
compare_vec (const std::vector <T>& lhs,
             const std::vector <T>& rhs)
{
    for (std::size_t i = 0; i < lhs.size (); ++i) {
        if (lhs[i] != rhs[i]) {
            return lhs[i] < rhs[i];
        }
    }

    return false;
}

template <typename T>
auto
find_min_row (const std::vector <std::vector <T>>& matrix)
{
    std::size_t N = matrix.size ();
    auto sorted_matrix = matrix;
    for (auto& row : sorted_matrix) {
        std::sort (row.begin (), row.end ());
    }

    auto min_row = std::min_element (sorted_matrix.cbegin (),
                                      sorted_matrix.cend (),
                                      compare_vec <T>);

    const auto index_min_row = min_row - sorted_matrix.cbegin ();

    return std::pair {index_min_row, std::move (*min_row)};
}

template <typename PosT = std::size_t, typename T>
std::vector <PosT>
get_sort_map (const std::vector <T>& source)
{
    const auto size = source.size ();

    std::vector <std::pair <T, PosT>> indexed_source;
    indexed_source.reserve (size);
    for (std::size_t i = 0; i < size; ++i) {
        indexed_source.push_back ({source[i], i});
    }

    std::sort (indexed_source.begin (), indexed_source.end (),
        [] (const auto& lhs, const auto& rhs) {
            return lhs.first < rhs.first;
        }
    );

    std::vector <PosT> indexes;
    indexes.reserve (size);
    for (const auto& [value, index] : indexed_source) {
        indexes.push_back (index);
    }

    return indexes;
}

template <typename T, typename PosT>
void
apply_map (std::vector <T>& vec,
           const std::vector <PosT>& index_map)
{
    const auto size = vec.size ();

    std::vector <T> buf (size);
    for (std::size_t i = 0; i < size; ++i) {
        buf[i] = std::move (vec[index_map[i]]);
    }

    std::swap (buf, vec);
}

template <typename T>
std::ostream&
print_matrix (const std::vector <std::vector <T>>& matrix,
              std::ostream& os = std::cout)
{
    for (const auto& row : matrix) {
        os << row << std::endl;
    }

    return os;
}

template <typename T>
void
solve (std::vector <T>& matrix)
{
    auto [min_row_i, sorted_min_row] = find_min_row (matrix);

    const auto index_sorted_map = get_sort_map (matrix[min_row_i]);

    matrix[min_row_i] = std::move (matrix[0]);
    matrix[0] = std::move (sorted_min_row);

    for (std::size_t i = 1; i < matrix.size (); ++i) {
        apply_map (matrix[i], index_sorted_map);
    }

    std::sort (matrix.begin () + 1, matrix.end (),
        [] (const auto& lhs, const auto& rhs) {
            const std::size_t size = lhs.size ();
            
            for (std::size_t i = 0; i < size; ++i) {
                if (lhs[i] != rhs[i]) {
                    return lhs[i] < rhs[i];
                }
            }

            return false;
        }
    );
}

template <typename T>
std::vector <std::vector <T>>
read_input_params ()
{
    std::size_t N = 0, M = 0;
    std::cin >> N >> M;

    return get_matrix <T> (N, M);
}

int main () {
    auto matrix = read_input_params <unsigned> ();

    // const std::size_t N = 50000, M = 5000;
    
    // seclib::RandomGenerator rand;
    // std::vector <std::vector <int>> matrix;
    // matrix.reserve (N);
    // for (std::size_t i = 0; i < N; ++i) {
    //     matrix.emplace_back (rand.get_vector <int> (M, 1200));
    // }

    solve  (matrix);
    print_matrix (matrix);

    // std::cout << matrix[4][7] << std::endl;

    return 0;
}