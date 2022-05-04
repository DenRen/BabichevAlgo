#include "../geoma/geoma.hpp"

template <typename T>
gtr::Line <T>
read_line_2_point (std::istream& is) {
    gtr::Vector <T> p, q;
    is >> p >> q;
    return gtr::Line <T> {p, q};
}

template <typename T>
std::ostream&
solve (std::istream& is,
       std::ostream& os) {
    gtr::Vector <T> p, q;
    is >> p >> q;

    gtr::Line <T> line {p, q};
    const auto line_abs_sq = line.a * line.a + line.b * line.b;
    const auto co_line = q - p;
    const auto co_line_abs_sq = co_line.abs_square ();

    std::size_t num_cake = 0;
    is >> num_cake;

    for (std::size_t i = 0; i < num_cake; ++i) {
        gtr::Circle <T> circ;
        is >> circ;
        
        auto dp = p - circ.p;
        auto dot = co_line * dp;
        T sign = dot >= 0 ? 1 : -1;
        auto r_sq = std::pow (circ.r, 2);
        if (co_line_abs_sq * r_sq <= sign * std::pow (dot, 2)) {
            continue;
        }

        if (dot >= 0) { // Half circle
            if (gtr::dist_square (circ.p, p) >= r_sq) {
                continue;
            }
        } else {        // Rectangle
            auto ro = std::abs (line.a * circ.p.x + line.b * circ.p.y + line.c);
            if (std::pow (ro, 2) >= line_abs_sq * r_sq) {
                continue;
            }
        }

        os << i + 1 << '\n';
    }

    return os;
}

int main () {
    std::ios_base::sync_with_stdio (false);

    solve <int> (std::cin, std::cout);
}