#include "print_lib.hpp"

using std::cout;
using std::endl;

int main () {
    std::tuple t (1, 2, std::tuple (std::string ("DED32"), "lol", "Lunev easy"), false);
    cout << t << endl;

    print_all_sep (t, 2, 4);
}