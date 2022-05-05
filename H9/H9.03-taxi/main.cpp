#include "../geoma/geoma.hpp"
#include "solve.hpp"

struct Client {
    gtr::Vector <double> pos;
    int id;

    Client () = default;

    Client (gtr::Vector <double>& pos,
            int id) :
        pos (pos), id (id)
    {}

    bool
    operator < (const Client& rhs) const noexcept {
        return pos < rhs.pos;
    }
};

std::ostream&
operator << (std::ostream& os,
             const Client& cli) {
    return os << '[' << cli.id << ':' << cli.pos << ']';
}

std::vector <std::pair <int, int>>
solve (int R,
       std::vector <Client>& clients) {
    std::sort (clients.begin (), clients.end ());
    
    std::vector <std::pair <int, int>> num_client (clients.size ());

    const auto R_sq = R * R;
    const auto n = clients.size ();
    for (std::size_t i = 0; i < n; ++i) {
        auto id = clients[i].id;
        num_client[i].first = id;
        num_client[i].second = 0;

        const auto& cur = clients[i];
        const auto[x0, y0] = cur.pos;

        for (std::size_t j = i - 1; j + 1 > 0; --j) {
            const auto& cli = clients[j];
            if (cli.pos.x < x0 - R) {
                break;
            }

            if (auto r = gtr::dist_square (cli.pos, cur.pos) - R_sq;
                r < 0 || r < 1e-6) {
                ++num_client[i].second;
            }
        }

        for (std::size_t j = i + 1; j < n; ++j) {
            const auto& cli = clients[j];
            if (cli.pos.x > x0 + R) {
                break;
            }

            if (auto r = gtr::dist_square (cli.pos, cur.pos) - R_sq;
                r < 0 || r < 1e-6) {
                ++num_client[i].second;
            }
        }
    }

    DUMP (num_client);

    std::sort (num_client.rbegin (), num_client.rend (),
        [] (const auto& lhs, const auto& rhs) {
            return lhs.second < rhs.second ||
                   (lhs.second == rhs.second && lhs.first > rhs.first);
    });

    num_client.resize (std::min <decltype (n)> (10, n));
    return num_client;
}

int main () {
    std::ios_base::sync_with_stdio (false);

    int N = 0;
    double R = 0;
    std::cin >> N >> R;

    std::vector <Client> clients (N);
    int i = 0;
    for (auto&[pos, index] : clients) {
        std::cin >> pos;
        index = i++;
    }

    auto res = solve (R, clients);
    for (const auto&[id, count] : res) {
        std::cout << id << ' ' << count << '\n';
    }
}