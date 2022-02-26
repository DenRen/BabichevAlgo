#include <fstream>
#include <vector>

#include "../../../libs/print_lib.hpp"

#if 1
    #define dump(obj) std::cout << #obj ": " << obj << std::endl
#else
    #define dump(obj)
#endif

int main () {
    std::string rfile_name = "read.txt";
    std::string wfile_name = "write.txt";

    std::fstream rs {rfile_name, std::ios_base::in};
    std::fstream ws {wfile_name, std::ios_base::out};

    std::string str_tmp;
    for (int i = 0; i < 5; ++i) {
        rs >> str_tmp;
        ws << str_tmp << "\n";
    }

    dump (rs.eof ());
    rs.clear ();
    dump (rs.eof ());
/*    while (std::getline (rs, str_tmp).good ()) {
        ws << str_tmp << "\n";    
    }
*/

    rs.seekg (0, std::ios::beg);
    str_tmp.erase ();

    rs >> str_tmp;
    dump (str_tmp);
    return 0;
    while (std::getline (rs, str_tmp).good ()) {
        std::cout << str_tmp << "\n";
    }
}
