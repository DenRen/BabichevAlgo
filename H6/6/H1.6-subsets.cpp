#include <iostream>
#include <vector>
#include <map>
#include <cstdio>
#include <sstream>

void print_subsets (const std::vector <std::tuple <char, int>>& set,
                    std::vector <std::tuple <char, int>>::const_iterator pos,
                    char* str, int len = 0)
{
    if (pos != set.cend ()) {
        const auto& pair = *pos++;

        auto symbol = std::get <char> (pair);
        auto repeat = std::get <int> (pair);

        for (int i = 0; i <= repeat; ++i) {
            print_subsets (set, pos, str, len);
            str[len++] = symbol;
        }
    } else {
        str[len] = '\0';
        printf ("%s\n", str);
    }
}

void solve (std::string input_set) {
    std::map <char, int> _set;
    for (const auto& item : input_set) {
        ++_set[item];
    }

    std::vector <std::tuple <char, int>> set;
    set.reserve (_set.size ());
    for (const auto& pair : _set) {
        set.push_back (pair);
    }

    char str[input_set.size () + 2];
    print_subsets (set, set.cbegin (), str);
}

int main () {
    std::string set;
    std::cin >> set;

    solve (set);
}
