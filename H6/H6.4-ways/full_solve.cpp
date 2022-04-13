#include <iostream>
#include <vector>
#include <tuple>
#include <set>

struct coord_t {
    int x, y;

    coord_t (int x = 0, int y = 0) :
        x (x), y (y)
    {}

    bool
    operator < (const coord_t& rhs) const noexcept {
        return x < rhs.x || (x == rhs.x && y < rhs.y);
    }
};

std::ostream&
operator << (std::ostream& os, const coord_t& coord) {
    return os << '{' << coord.x << ',' << coord.y << '}';
}

template <typename T>
void
print_map (const std::vector <std::vector <T>>& map) {
    for (const auto& row : map) {
        for (T item : row) {
            std::cout << item << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

std::tuple <std::vector <std::vector <bool>>, coord_t, coord_t, int>
read_lab (std::istream& is = std::cin) {
    int num_rows = 0, num_cols = 0, num_iters = 0;
    is >> num_rows >> num_cols >> num_iters;

    coord_t start = {}, finish = {};
    std::vector <std::vector <bool>> lab_map (num_rows + 2);

    for (auto& row : lab_map) {
        row = std::vector <bool> (num_cols + 2);
    }

    std::string tmp_str;
    for (int i_row = 0; i_row < num_rows; ++i_row) {
        is >> tmp_str;
        for (int i_col = 0; i_col < num_cols; ++i_col) {
            char symb = tmp_str[i_col];

            lab_map[i_row + 1][i_col + 1] = (symb != '+');

            if (symb == '@') {
                start.x = i_col + 1;
                start.y = i_row + 1;
            } else if (symb == '#') {
                finish.x = i_col + 1;
                finish.y = i_row + 1;
            }
        }
    }

    return {lab_map, start, finish, num_iters};
}

int
is_possible (const std::vector <std::vector <bool>>& bit_map,
             const std::vector <std::vector <std::size_t>>& num_map,
             int i_row, int i_col,
             int offset_row, int offset_col)
{
    i_row += offset_row;
    i_col += offset_col;
    return bit_map[i_col][i_row] && num_map[i_col][i_row];
}

void
add_if_possible (const std::vector <std::vector <bool>>& bit_map,
                 const std::vector <std::vector <std::size_t>>& prev_num_map,
                 std::vector <std::vector <std::size_t>>& num_map,
                 std::set <coord_t>& new_fire,
                 const coord_t& p,
                 int off_x, int off_y)
{
    int x = p.x + off_x, y = p.y + off_y;
    if (bit_map[y][x]) {
        new_fire.emplace (x, y);
        num_map[y][x] += prev_num_map[p.y][p.x];
    }
}

std::size_t
solve (std::vector <std::vector <bool>>& bit_map,
       const coord_t& start,
       const coord_t& finish,
       int num_iters)
{ 
    const auto num_col = bit_map.size ();
    const auto num_row = bit_map[0].size ();

    std::vector <std::vector <std::size_t>> num_map (bit_map.size ());
    for (auto& row : num_map) {
        row = std::vector <std::size_t> (bit_map[0].size ());
    }

    auto prev_num_map = num_map;
    prev_num_map[start.y][start.x] = 1;

    std::set <coord_t> prev_fire, cur_fire;
    prev_fire.insert (start);

    for (int i_iter = 0; i_iter < num_iters; ++i_iter) {
        for (const coord_t& p : prev_fire) {
            auto check = [&] (int offset_x, int offset_y) {
                add_if_possible (bit_map, prev_num_map, num_map, cur_fire, p,
                                 offset_x, offset_y);
            };
    
            check (-1,  0);
            check ( 1,  0);
            check ( 0, -1);
            check ( 0,  1);
        }

        std::swap (prev_fire, cur_fire);
        cur_fire.clear ();

        std::swap (num_map, prev_num_map);
        for (auto& row : num_map) {
            std::fill (row.begin (), row.end (), 0);
        }
    }
    
    return prev_num_map[finish.y][finish.x];
}

int main () {
    std::ios_base::sync_with_stdio (false);

    auto[lab, start, finish, num_iters] = read_lab (std::cin);
    std::cout << solve (lab, start, finish, num_iters) << '\n'; 
}
