#include "../geoma/geoma.hpp"
#include <iomanip>

std::tuple <double, int, int>
solve (std::vector <std::pair <gtr::Vector <double>, int>>& ps) {
    std::sort (ps.begin (), ps.end (),
        [] (const auto& lhs, const auto& rhs) {
            return lhs.first < rhs.first;
    });

    double min_dist = std::numeric_limits <double>::max ();
    int min_dist_i = 0, min_dist_j = 0;

    const long size = ps.size ();
    for (int i = 0; i < size; ++i) {
        const auto& p = ps[i].first;
        for (int j = i - 1; j >= 0; --j) {
            double dx_sq = std::pow (p.x - ps[j].first.x, 2);
            if (dx_sq >= min_dist) {
                break;
            }

            double dist = gtr::dist_square (p, ps[j].first);
            if (dist < min_dist) {
                min_dist = dist;
                min_dist_i = ps[i].second;
                min_dist_j = ps[j].second;
            }
        }

        for (int j = i + 1; j < size; ++j) {
            double dx_sq = std::pow (p.x - ps[j].first.x, 2);
            if (dx_sq >= min_dist) {
                break;
            }

            double dist = gtr::dist_square (p, ps[j].first);
            if (dist < min_dist) {
                min_dist = dist;
                min_dist_i = ps[i].second;
                min_dist_j = ps[j].second;
            }
        }
    }

    if (min_dist_i > min_dist_j) {
        std::swap (min_dist_i, min_dist_j);
    }

    return {std::sqrt (min_dist), min_dist_i, min_dist_j};
}

int main () {
    std::ios_base::sync_with_stdio (false);

    int n = 0;
    std::cin >> n;

    std::vector <std::pair <gtr::Vector <double>, int>> ps (n);
    int counter = 1;
    for (auto&[p, id] : ps) {
        id = counter++;
        std::cin >> p;
    }

    auto[dist, i, j] = solve (ps);
    std::cout << std::setprecision (11) << dist
              << ' ' << i << ' ' << j << '\n';
}