#include "solve.hpp"

#include <algorithm>
#include <iostream>

#include "../../libs/other_func.hpp"

data_t
next_seq_elem (data_t x,
               data_t M,
               data_t L)
{
    unsigned long long M_ = M;
    return (unsigned int) ((x * M_) & 0xFFFFFFFFU) % L;
}

std::vector <data_t>
calc_seq (data_t N,
          data_t K,
          data_t M,
          data_t L)
{
    std::vector <data_t> vec (N);

    vec[0] = K;
    for (std::size_t i = 0; i < N-1; i++) {
        vec[i + 1] = next_seq_elem (vec[i], M, L);
    }

    return vec;
}

data_t
sum_seq_even (const std::vector <data_t>& seq,
              data_t L)
{
    data_t sum = 0;
    for (std::size_t i = 0; i < seq.size (); i += 2) {
        sum += seq[i];
    }
    sum %= L;

    return sum;
}

data_t
ref_solve (data_t N,
           data_t K,
           data_t M,
           data_t L)
{
    std::vector <data_t> seq = calc_seq (N, K, M, L);
    std::sort (seq.begin (), seq.end ());
    data_t sum = sum_seq_even (seq, L);

    return sum;
}

void
sort_count (std::vector <data_t>& seq,
            data_t L)
{
    // std::cout << "size: " << seq.size () << std::endl;
    // std::cout << "seq: " << seq << std::endl;

    data_t save_first = seq[0];
    bool big_first = save_first >= L;
    if (big_first) {
        seq[0] = L - 1;
    }

    std::vector <data_t> count (L);
    for (auto val : seq) {
        ++count[val];
    }

    // std::cout << "count: " << count << std::endl;

    std::size_t count_size = 0;
    std::size_t seq_i = 0;
    for (std::size_t i = 0; i < L; ++i) {
        data_t cur_count = count[i];
        if (cur_count != 0) {
            ++count_size;
            while (cur_count--) {
                seq[seq_i++] = i;
            }
        }
    }

    std::cout << 100 * (double) count_size / seq.size () << "%" << std::endl;

    if (big_first) {
        seq[seq.size () - 1] = save_first;
    }

    // std::cout << "sorted seq: " << seq << std::endl;
}

void
seq_sort (std::vector <data_t>& seq,
          data_t L)
{
    data_t N = seq.size ();
    if (N > 2 * L) {
        std::cout << "sort_count" << std::endl;
        sort_count (seq, L);
    } else {
        std::cout << "std sort" << std::endl;
        std::sort (seq.begin (), seq.end ());
    }
}

data_t
solve (data_t N,
       data_t K,
       data_t M,
       data_t L)
{
    std::vector <data_t> seq = calc_seq (N, K, M, L);
    seq_sort (seq, L);
    data_t sum = sum_seq_even (seq, L);

    return sum;
}

data_t
sum_seq_even (const std::map <data_t, unsigned>& map,
              data_t L)
{
    data_t result = 0;
    bool p = 0;
    for (const auto& [value, freq] : map) {
        data_t num = (freq + !p) / 2;
        p = !((freq + !p) % 2);
        
        result += value * num;
    }

    return result % L;
}

data_t
solve_from_map (data_t N,
                data_t K,
                data_t M,
                data_t L)
{
    std::map <data_t, unsigned> map;
    data_t prev_value = K;
    ++map[prev_value];
    for (data_t i = 1; i < N; ++i) {
        prev_value = next_seq_elem (prev_value, M, L);
        ++map[prev_value];
    }

    return sum_seq_even (map, L);
}

void insertion_sort (data_t* array, int offset, int end) {
    int x, y, temp;
    for (x = offset; x < end; ++x) {
        for (y = x; y > offset && array[y-1] > array[y]; y--) {
            temp = array[y];
            array[y] = array[y-1];
            array[y-1] = temp;
        }
    }
}

void radix_sort (data_t* array, int offset, int end, int shift) {
    int last[256] = { 0 }, pointer[256];

    for (int x = offset; x < end; ++x) {
        ++last[(array[x] >> shift) & 0xFF];
    }

    last[0] += offset;
    pointer[0] = offset;
    for (data_t x = 1; x < 256; ++x) {
        pointer[x] = last[x-1];
        last[x] += last[x-1];
    }

    for (data_t x = 0; x < 256; ++x) {
        while (pointer[x] != last[x]) {
            data_t value = array[pointer[x]];
            data_t y = (value >> shift) & 0xFF;
            while (x != y) {
                data_t temp = array[pointer[y]];
                array[pointer[y]++] = value;
                value = temp;
                y = (value >> shift) & 0xFF;
            }
            array[pointer[x]++] = value;
        }
    }

    if (shift > 0) {
        shift -= 8;
        for (data_t x = 0; x < 256; ++x) {
            data_t temp = x > 0 ? pointer[x] - pointer[x-1] : pointer[0] - offset;
            if (temp > 64) {
                radix_sort (array, pointer[x] - temp, pointer[x], shift);
            } else if (temp > 1) {
                insertion_sort (array, pointer[x] - temp, pointer[x]);
            }
        }
    }
}

data_t
solve_from_radix (data_t N,
                  data_t K,
                  data_t M,
                  data_t L)
{
    auto seq = calc_seq (N, K, M, L);
    radix_sort (seq.data (), 0, N, 24);
    data_t sum = sum_seq_even (seq, L);
    
    return sum;
}