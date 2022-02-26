#include <fstream>
#include <iostream>

int main () {
    std::fstream fs {"line.txt", std::ios_base::in};
    
    std::cout << fs.tellg () << std::endl;
    
    std::string str;
    fs >> str;

    std::cout << fs.tellg () << std::endl;
}
