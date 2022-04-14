#include <iostream>

std::size_t
solve () {
    int N = 0;
    std::cin >> N;

    std::size_t Na = 0, Ma = 0, Mb = 0;
    std::cin >> Na >> Ma >> Mb;

    std::size_t Sa = 0, Sb = 0;
    for (int i = 0; i < N - 3; ++i) {
        std::size_t Mc = 0;
        std::cin >> Mc;
        
        std::size_t S1 = Sb + Na * Ma * Mb + Na * Mb * Mc;
        std::size_t S2 = Ma * Mb * Mc + Na * Ma * Mc;
        if (S1 < S2) {
            Sa += Sb + Na * Ma * Mb;
            Ma = Mb;
        } else {
            Sb += Ma * Mb * Mc;
        }

        Mb = Mc;
   }

    return Sa + Sb + Na * Ma * Mb;
}

int main () {
    std::ios_base::sync_with_stdio (false);

    std::cout << solve () << '\n';
    return 0;
}
