#include <iostream>
#include <algorithm>

int main () {
    std::string_view str = "ab\ncd";

    std::cout << "\"" << std::find (str.begin () + 3, str.end (), '\n')
              << "\"" << std::endl;
}