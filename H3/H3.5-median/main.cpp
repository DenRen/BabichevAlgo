#include "solve.hpp"

int main () {
    unsigned num_req = 0;
    if (scanf ("%u\n", &num_req) != 1) {
        throw std::invalid_argument ("Failed read number reqs");
    }
    exec_requests (num_req);
}