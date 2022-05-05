#include "geoma.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);
    
    double a = 3;
    double b = 3;

    gtr::is_equal (a, b);
    // gtr::is_equal ((float) a, b);
    // gtr::is_equal ((float) a, (float) b);
}