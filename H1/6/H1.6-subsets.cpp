#include <iostream>
#include <vector>
#include <map>

void func (const std::vector <std::tuple <char, int>>& set,
           std::vector <std::tuple <char, int>>::const_iterator pos,
           std::string cur = "")
{
    if (pos != set.cend ()) {
        const auto& pair = *pos++;

        auto symbol = std::get <char> (pair);
        auto repeat = std::get <int> (pair);

        for (int i = 0; i <= repeat; ++i) {
            func (set, pos, cur);
            cur += symbol;
        }
    } else {
        std::cout << cur << std::endl;
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
    
    func (set, set.cbegin ());
}
int main () {
    std::string set;
    std::cin >> set;

    solve (set);
}
