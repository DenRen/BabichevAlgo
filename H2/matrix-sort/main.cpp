#include <iostream>
#include <vector>
#include <iterator>
#include <type_traits>
#include <algorithm>

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
class matrix_uniq {
    std::vector <T> data_;
    std::size_t row_size_;

    std::pair <std::size_t, std::size_t>
    position2coord (std::size_t pos) const noexcept {
        return std::pair {pos / row_size_, pos % row_size_};
    }

public:
    matrix_uniq (std::istream& is) {
        std::size_t num_rows = 0;
        is >> num_rows >> row_size_;

        const auto size = num_rows * row_size_;
        data_.reserve (size);

        std::istream_iterator <T> it_input {is};
        std::copy_n (it_input, size, std::back_inserter (data_));

        if (is.fail ()) {
            throw std::runtime_error ("Failed read matrix!");
        }
    }

    class proxy_row {
        T* data_;
        std::size_t size_;
    
    public:
        proxy_row (T* data,
                   std::size_t size) noexcept :
            data_ (data),
            size_ (size)
        {}

        const T&
        operator[] (std::size_t index_col) const noexcept {
            return data_[index_col];
        }

        T&
        operator[] (std::size_t index_col) noexcept {
            return const_cast <T&> (
                static_cast <const proxy_row&>
                    (*this)[index_col]
            );
        }

        const T*
        cbegin () const noexcept {
            return data_;
        }

        const T*
        cend () const noexcept {
            return data_ + size_;
        }
    }; // class proxy_row

    proxy_row
    operator[] (std::size_t index_row) /* const */ noexcept {
        return proxy_row {data_.data () + index_row * row_size_, row_size_};
    }

    void
    dump (std::ostream& os = std::cout) {
        const auto num_cols = row_size_;
        const auto num_rows = this->num_rows ();
        
        for (std::size_t i = 0; i < num_rows; ++i) {
            for (std::size_t j = 0; j + 1 < num_cols; ++j) {
                os << (*this)[i][j] << " ";
            }
            os << (*this)[i][num_cols - 1] << std::endl;
        }
    }

    std::size_t
    num_cols () const noexcept {
        return row_size_;
    }

    std::size_t
    num_rows () const noexcept {
        return data_.size () / row_size_;
    }

    std::pair <std::size_t, std::size_t>
    find_min () const {
        const auto it_min_el = std::min_element (data_.cbegin (), data_.cend ());
        const auto pos_min_el = it_min_el - data_.cbegin ();
        return position2coord (pos_min_el);
    }

    T*
    data () const noexcept {
        return data_.data ();
    }
}; // class matrix_uniq

template <typename PosT = std::size_t, typename Iterator>
std::pair <std::vector <PosT>,
           std::vector <typename std::iterator_traits <Iterator>::value_type>>
calc_index_map (Iterator begin,
                Iterator end)
{
    using T = typename std::iterator_traits <Iterator>::value_type;
    const auto size = std::distance (begin, end);

    std::vector <std::pair <T, PosT>> indexed_vec;
    indexed_vec.reserve (size);
    for (std::size_t i = 0; i < size; ++i) {
        indexed_vec.push_back ({begin[i], i});
    }

    std::sort (indexed_vec.begin (), indexed_vec.end (),
        [] (const auto& lhs, const auto& rhs) {
            return lhs.first < rhs.first;
        }
    );

    std::vector <PosT> indexes;
    std::vector <T> sorted;
    indexes.reserve (size);
    sorted.reserve (size);
    for (const auto& [value, index] : indexed_vec) {
        indexes.push_back (index);
        sorted.push_back (value);
    }

    return {indexes, sorted};
}

template <typename InputIterator, typename OutputIterator,
          typename PosT>
void
apply_map (InputIterator src,
           OutputIterator dest,
           const std::vector <PosT>& index_map)
{
    for (std::size_t i = 0; i < index_map.size (); ++i) {
        dest[i] = src[index_map[i]];
    }
}

int main () {
    using data_t = unsigned;

    matrix_uniq <data_t> matrix {std::cin};

    const auto min_elem_pos = matrix.find_min ();
    const auto num_rows = matrix.num_rows ();

    // Calc vertical order
    std::vector <std::pair <data_t, unsigned>> order;
    order.reserve (num_rows);
    for (std::size_t row = 0; row < num_rows; ++row) {
        order.push_back ({matrix[row][min_elem_pos.second], row});
    }

    std::sort (order.begin (), order.end (),
        [] (const auto& lhs, const auto& rhs) {
            return lhs.first < rhs.first;
        }
    );

    // Calc index map and print first row
    const std::size_t index_min_row = min_elem_pos.first;
    const auto[index_map, sorted_min_row] = calc_index_map (
        matrix[index_min_row].cbegin (),
        matrix[index_min_row].cend ()
    );
    std::cout << sorted_min_row << std::endl;
    
    std::vector <data_t> print_buf (matrix.num_cols ());
    for (const auto&[_, row] : order) {
        if (row == index_min_row) {
            continue;
        }
        
        apply_map (matrix[row].cbegin (), print_buf.begin (), index_map);
        std::cout << print_buf << std::endl;
    }
}