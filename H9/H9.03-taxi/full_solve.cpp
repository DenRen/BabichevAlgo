#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
#include <complex>

// g++ -DHOST -std=c++17 main.cpp

#define NDEBUG
// #define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) std::cerr << #obj ": " << obj << '\n'
#else
    #define DUMP(obj)
#endif

namespace gtr {

template <typename T>
struct Vector {
    T x, y;

    Vector (T x = 0, T y = 0) :
        x {x}, y {y}
    {}

    Vector (const Vector&) = default;
    Vector (Vector&&) = default;

    Vector&
    operator = (const Vector&) = default;
    Vector&
    operator = (Vector&&) = default;

    bool
    operator < (const Vector& rhs) const noexcept {
        return x < rhs.x || (x == rhs.x && y < rhs.y);
    }

    Vector&
    operator += (const Vector& rhs) noexcept {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector&
    operator -= (const Vector& rhs) noexcept {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector
    operator - (int) const noexcept {
        return Vector {-x, -y};
    }

    template <typename U>
    Vector&
    operator *= (U k) noexcept {
        x *= k;
        y *= k;
        return *this;
    }

    template <typename U>
    Vector&
    operator /= (U k) noexcept {
        x /= k;
        y /= k;
        return *this;
    }

    template <typename U>
    auto
    operator * (const Vector <U>& p) const noexcept {
        return x * p.x + y * p.y;
    }

    template <typename U>
    auto
    operator ^ (const Vector <U>& p) const noexcept {
        return x * p.y - p.x * y;
    }

    T
    abs_square () const noexcept {
        return x * x + y * y;
    }

    T
    abs () const noexcept {
        return std::sqrt (abs_square ());
    }
};

template <typename T, typename U>
auto
operator + (const Vector <T>& p1, const Vector <U>& p2) {
    auto p {p1};
    return p += p2;
}

template <typename T, typename U>
auto
operator - (const Vector <T>& p1, const Vector <U>& p2) {
    auto p {p1};
    return p -= p2;
}

template <typename T>
auto
dist (const Vector <T>& p1,
      const Vector <T>& p2) {
    auto p {p1};
    p -= p2;
    return p.abs ();
}

template <typename T>
auto
dist_square (const Vector <T>& p1,
             const Vector <T>& p2) {
    auto p {p1};
    p -= p2;
    return p.abs_square ();
}

template <typename T>
struct Line {
    T a, b, c;

    Line (T a = 0, T b = 0, T c = 0) :
        a {a}, b {b}, c {c}
    {}

    Line (const Vector <T>& p,
          const Vector <T>& q) :
          a {q.y - p.y},
          b {p.x - q.x},
          c {q.x * p.y - p.x * q.y}
    {}

    Line (const Line&) = default;
    Line (Line&&) = default;

    Line&
    operator = (const Line&) = default;
    Line&
    operator = (Line&&) = default;

    Vector <T>
    unnorm_normal () const noexcept {
        return {a, b};
    }

    Vector <T>
    normal () const {
        return unnorm_normal () / std::sqrt (a * a + b * b);
    }

    Vector <T>
    unnorm_co_dir () const noexcept {
        return {b, -a};
    }

    Vector <T>
    co_dir () const {
        return unnorm_co_dir () / std::sqrt (a * a + b * b);
    }
};

template <typename T>
T
dist_square (const Line <T>& line,
             const Vector <T>& point) {
    auto u = line.a * point.x + line.y * point.y + line.c;
    return u * u / (line.a * line.a + line.b * line.b);
}

template <typename T>
T
dist (const Line <T>& line,
      const Vector <T>& point) {
    auto u = line.a * point.x + line.y * point.y + line.c;
    return std::abs (u) / std::sqrt (line.a * line.a + line.b * line.b);
}

template <typename T>
struct Triangle {
    std::array <Vector <T>, 3> vs;

    Triangle () = default;

    Triangle (const Vector <T>& p1,
              const Vector <T>& p2,
              const Vector <T>& p3) :
        vs {p1, p2, p3}
    {}

    T
    area2 () const noexcept {
        auto p1 = vs[1] - vs[0];
        auto p2 = vs[2] - vs[0];
        return std::abs (p1 ^ p2);
    }

    T
    area () const {
        return area2 () / 2;
    }
};

template <typename T>
std::enable_if_t <std::is_integral_v <T>, bool>
vector_in_triangle (const Triangle <T>& triangle,
                    const Vector <T>& point) noexcept {
    T S = 0;
    for (int i = 0; i < 3; ++i) {
        auto vec_to_point = point - triangle.vs[i];
        auto edge = triangle.vs[(i + 1) % 3] - triangle.vs[i];

        auto cross_mult = vec_to_point ^ edge;
        S += std::abs (cross_mult);
    }

    return S == triangle.area2 ();
}

template <typename T>
std::ostream&
operator << (std::ostream& os,
             const Vector <T>& p) {
    return os << '{' << p.x << ", " << p.y << '}';
}

template <typename T>
std::istream&
operator >> (std::istream& is,
             Vector <T>& p) {
    return is >> p.x >> p.y;
}

template <typename T>
std::ostream&
operator << (std::ostream& os,
             const Triangle <T>& tr) {
    return os << '{' << tr.vs[0] << ", "
                     << tr.vs[1] << ", "
                     << tr.vs[2] << '}';
}

template <typename T>
std::istream&
operator >> (std::istream& is,
             Triangle <T>& tr) {
    return is >> tr.vs[0] >> tr.vs[1] >> tr.vs[2];
}

template <typename T>
struct Circle {
    Vector <T> p;
    T r;

    Circle (const Vector <T>& center = {},
            T radius = {}) :
        p (center),
        r (radius)
    {}
};

template <typename T>
std::istream&
operator >> (std::istream& is,
             Circle <T>& circ) {
    return is >> circ.p >> circ.r;
}

template <typename T>
std::ostream&
operator << (std::ostream& os,
             const Circle <T>& circ) {
    return os << '(' << circ.p << ", " << circ.r << ')';
}

} // namespace gtr

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