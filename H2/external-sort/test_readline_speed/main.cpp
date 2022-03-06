#include <fstream>
#include <iostream>

int main () {
    const std::string file_name = "input.txt";

    std::ios_base::sync_with_stdio (false);

    int j = 0;
    for (int i = 0; i < 40; ++i) {

#ifdef CPP
    std::fstream file {file_name, std::ios_base::in};

    // char* str_buf = new char[10000];
    std::string str_buf;
    // str_buf.reserve (10000);
    // while (file.getline (str_buf.data (), 10000).good ()) {
    // while (file.getline (str_buf, 10000).good ()) {
    while (std::getline (file, str_buf).good ()) {
        // if (str_buf[0] == '=') {
        //     std::cout << "lol\n";
        // }
        ++j;
    }
    // delete str_buf;
#else

    FILE* file = fopen (file_name.data (), "r");
    if (file == nullptr) {
        throw std::runtime_error ("fopen");
    }

    char* str_buf = new char[10000];

    while (fgets (str_buf, 10000, file) != NULL) {
        // if (str_buf[0] == '=') {
        //     std::cout << "lol\n";
        // }
        ++j;
    }

    delete[] (str_buf);
    fclose (file);

#endif

    }
    
    std::cout << j << std::endl;
}