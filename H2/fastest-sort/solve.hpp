#pragma once
#include <vector>
#include <map>

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

data_t
solve (data_t N,    // Sequence size
       data_t K,    // First value (seq[0] = K)
                    // A_i+1 = (A_i * M) % 2^32 % L;
       data_t M,    // In formuls ----^           ^
       data_t L);   // In formula ----------------/

data_t
solve_from_map (data_t N,
                data_t K,
                data_t M,
                data_t L);

void
sort_count (std::vector <data_t>& seq,
            data_t L);
data_t
next_seq_elem (data_t x,
               data_t M,
               data_t L);

data_t
sum_seq_even (const std::map <data_t, unsigned>& map,
              data_t L);

data_t
solve_from_radix (data_t N,
                  data_t K,
                  data_t M,
                  data_t L);