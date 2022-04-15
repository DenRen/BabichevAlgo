#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
std::ostream&
operator << (std::ostream& os,
             const std::vector <T>& vec)
{
    const std::size_t size = vec.size ();
    if (size == 0) {
        return os;
    }

    for (std::size_t i = 0; i + 1 < size; ++i) {
        os << vec[i] << " ";
    }

    return os << vec[size - 1];
}

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
    
    void
    fill_first_row_dist_map (std::vector <int>& dist_row) const {
        dist_row.resize (num_village);
        dist_row[0] = 0;
        for (int i_vill = 1; i_vill < num_village; ++i_vill) {
            int dist = vill_coord[i_vill] - vill_coord[i_vill - 1];
            dist_row[i_vill] = dist_row[i_vill - 1] + i_vill * dist;
        }
    }
    
    std::vector <std::vector <int>>
    get_left_dist_map () const {
        std::vector <std::vector <int>> dist_map (num_post);
        fill_first_row_dist_map (dist_map[0]);
        
        for (int i_post = 1; i_post < num_post; ++i_post) {
            dist_map[i_post].resize (num_village);
            std::fill_n (dist_map[i_post].begin (), i_post, INT32_MAX);
            
            for (int i_right_post = i_post; i_right_post < num_village; ++i_right_post) {
                int min_dist = INT32_MAX;
                for (int i = i_post - 1; i < i_right_post; ++i) {
                    // Calc dist
                    int dist = dist_map[i_post - 1][i];

                    int dist_betw_post = vill_coord[i_right_post] - vill_coord[i];
                    int j = i + 1;
                    for (; j < i_right_post; ++j) {
                        int dist_left = vill_coord[j] - vill_coord[i];
                        int dist_right = dist_betw_post - dist_left;
                        if (dist_left > dist_right) {
                            break;
                        }

                        dist += dist_left;
                    }

                    for (; j < i_right_post; ++j) {
                        dist += vill_coord[i_right_post] - vill_coord[j];
                    }

                    // Save min dist
                    if (dist < min_dist) {
                        min_dist = dist;
                    }
                }

                dist_map[i_post][i_right_post] = min_dist;
            }
        }

        return dist_map;
    }

    std::vector <int>
    get_dist_right_vec (int size) const {
        std::vector <int> dists (size);
        dists[size - 1] = 0;
        for (int i = size - 2; i >= 0; --i) {
            int dist = vill_coord[i + 1] - vill_coord[i];
            dists[i] = dists[i + 1] + dist * (size - 1 - i);
        }

        return dists;
    }

    std::pair <std::size_t, std::vector <int>>
    solve () const {
        auto dist_map = get_left_dist_map ();
       
        // Find last post pos
        auto& last_row = dist_map[num_post - 1];

        auto dists_right = get_dist_right_vec (num_village);
        for (int i = num_post - 1; i < num_village - 1; ++i) {
            last_row[i] += dists_right[i];         
        }

        int i_min_dist = num_post - 1, min_dist = last_row[i_min_dist];
        for (int i = num_post; i < num_village; ++i) {
            if (last_row[i] < min_dist) {
                min_dist = last_row[i];
                i_min_dist = i;
            }
        }

        const auto save_min_dist = min_dist;
        
        std::vector <int> pos_post;
        pos_post.push_back (i_min_dist);
       
        min_dist -= dists_right[i_min_dist];

        // Find [1, ..., num_post - 2] post poss
        for (int i_post = num_post - 2; i_post >= 0; --i_post) {
            int i_post_end = pos_post[pos_post.size () - 1];
            auto& dist_row = dist_map[i_post];
            for (int i = i_post_end - 1; i >= i_post; --i) {
                int dist_betw_post = vill_coord[i_post_end] - vill_coord[i];
                int dist = 0;
                for (int j = i + 1; j < i_post_end; ++j) {
                    // Find sum of dists between vill_coord[i] and vill_coord[i_post_end]
                    int dist_left = vill_coord[j] - vill_coord[i];
                    int dist_right = dist_betw_post - dist_left;
                    dist += std::min (dist_left, dist_right);
                }

                if (dist + dist_row[i] == min_dist) {
                    min_dist -= dist;
                    pos_post.push_back (i);
                    break;
                }
            }
        }

        std::reverse (pos_post.begin (), pos_post.end ());
        for (auto& pos : pos_post) {
            pos = vill_coord[pos];
        }

        return {save_min_dist, pos_post};
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
