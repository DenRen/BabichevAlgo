#include "solve.hpp"

int main () {
    auto points = read_points ();
    auto answer = solve (points);
    print_answer (answer);
}