#include "solve.hpp"

int main () {
    std::ios_base::sync_with_stdio (false);

    std::string str;
    std::getline (std::cin, str);
    std::cout << native_solve (str) << std::endl;
}