#include "solve.hpp"

#include <algorithm>

std::vector <data_t>
calc_seq (data_t N,
          data_t K,
          data_t M,
          data_t L)
{
    std::vector <data_t> vec (N);

    unsigned long long M_ = M;
    vec[0] = K;
    for (std::size_t i = 0; i < N-1; i++) {
        vec[i + 1] = (unsigned int) ((vec[i] * M_) & 0xFFFFFFFFU) % L;
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
sort_count (const std::vector <data_t>& seq,
            data_t L)
{

}

void
seq_sort (const std::vector <data_t>& seq,
          data_t L)
{
    data_t N = seq.size ();
    if (N > 10 * L) {
        sort_count (seq, L);
    } else {
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