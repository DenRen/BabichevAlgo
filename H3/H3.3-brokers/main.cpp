#include <iostream>
#include <map>
#include <iomanip>

double
native_solve ()
{
    std::size_t N = 0;
    std::cin >> N;

    std::map <double, int> freq_cap;
    for (std::size_t i = 0; i < N; ++i) {
        double capital = 0;
        std::cin >> capital;
        ++freq_cap[capital];
    }

    double sum = 0.0;
    while (freq_cap.size () != 1 || freq_cap.begin ()->second != 1) {
        double new_cap = 0;
        for (int i = 0; i < 2; ++i) {
            auto it = freq_cap.begin ();

            double cap = it->first;
            new_cap += cap;

            if (--(it->second) == 0) {
                freq_cap.erase (it);
            }
        }

        ++freq_cap[new_cap];

        sum += 0.01 * new_cap;
    }

    return sum;
}

int main () {
    std::cout << std::fixed << std::setprecision (2) << native_solve () << "\n";
}