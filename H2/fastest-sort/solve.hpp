#pragma once
#include <vector>

typedef unsigned int data_t;

std::vector <data_t>
calc_seq (data_t N,
          data_t K,
          data_t M,
          data_t L);

data_t
sum_seq_even (const std::vector <data_t>& seq,
              data_t L);

data_t
ref_solve (data_t N,    // Sequence size
           data_t K,    // First value (seq[0] = K)
                        // A_i+1 = (A_i * M) % 2^32 % L;
           data_t M,    // In formuls ----^           ^
           data_t L);   // In formula ----------------/