#include "solve.hpp"
#include "../../libs/print_lib.hpp"

class post_solver {
    int num_village, num_post;
    std::vector <int> vill_coord;

public:
    template <typename T>
    post_solver (int num_village,
                 int num_post,
                 T&& vill_coord) :
        num_village (num_village),
        num_post (num_post),
        vill_coord (std::forward <T> (vill_coord))
    {}

    std::size_t
    calc_full_dist (const std::vector <int>& post_coord,
                    int i_vill_end) const {
        std::size_t full_dist = 0;

        int i_vill = 0;
        for (; i_vill < i_vill_end; ++i_vill) {
            int dist = post_coord[0] - vill_coord[i_vill];
            if (dist >= 0) {
                full_dist += dist;
            } else {
                break;
            }
        }
        
        if (num_post > 1) {
            for (int i_post = 1; i_vill < i_vill_end; ++i_vill) {
                int dist_left = vill_coord[i_vill] - post_coord[i_post - 1];
                int dist_right = post_coord[i_post] - vill_coord[i_vill];

                full_dist += std::min (dist_left, dist_right);

                if (dist_right == 0) {
                    if (++i_post == num_post) {
                        ++i_vill;
                        break;
                    }
                }
            }
        }

        for (; i_vill < i_vill_end; ++i_vill) {
            full_dist += vill_coord[i_vill] - post_coord[num_post - 1];
        }

        return full_dist;
    }

    std::pair <std::size_t, std::vector <int>>
    solve () {
        if (num_post == num_village) {
            return {0, vill_coord};
        }

        std::vector <int> post_coord (num_post), post_pos (num_post);
        std::copy_n (vill_coord.cbegin (), num_post, post_coord.begin ());
        for (int i = 0; i < num_post; ++i) {
            post_pos[i] = i;
        }
        
        for (int i_vill = num_post; i_vill < num_village; ++i_vill) {
            int i_pos_max = i_vill;
            for (int i_post = num_post - 1; i_post >= 0; --i_post) {
                int post_pos_begin = post_pos[i_post];
                
                int dist_min = calc_full_dist (post_coord, i_vill + 1);
                int& dist_min_pos = post_pos[i_post];

                for (int i_pos = post_pos_begin + 1; i_pos <= i_pos_max; ++i_pos) {
                    post_coord[i_post] = vill_coord[i_pos];
                    auto dist = calc_full_dist (post_coord, i_vill + 1);

                    if (dist < dist_min) {
                        dist_min = dist;
                        dist_min_pos = i_pos;
                    }
                }

                post_coord[i_post] = vill_coord[dist_min_pos];

                if (post_pos_begin == dist_min_pos) {
                    break;
                }

                i_pos_max = dist_min_pos - 1;
            }
        }

        std::size_t full_dist = calc_full_dist (post_coord, num_village);
        return {full_dist, post_coord};
    }

};

int main () {
    std::ios_base::sync_with_stdio (false);

    int n = 0, m = 0;
    std::cin >> n >> m;

    std::vector <int> vill_coord (n);
    for (auto& coord : vill_coord) {
        std::cin >> coord;
    }

    post_solver solver {n, m, std::move (vill_coord)};
    auto[full_dist, post_coord] = solver.solve ();
    std::cout << full_dist << '\n' << post_coord << '\n';
}
