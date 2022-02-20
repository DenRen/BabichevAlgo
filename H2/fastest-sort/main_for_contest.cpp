#include <algorithm>
#include <iostream>

typedef unsigned int data_t;

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

int main () {
    data_t N = 0, K = 0, M = 0, L = 0;
    std::cin >> N >> K >> M >> L;

    data_t result = solve_from_radix (N, K, M, L);
    std::cout << result << std::endl;
}