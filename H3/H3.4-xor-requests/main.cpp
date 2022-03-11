#include "solve.hpp"

int main () {
    auto[arr, num_req] = read_input <int, unsigned> ();
    exec_requests (arr, num_req);
}