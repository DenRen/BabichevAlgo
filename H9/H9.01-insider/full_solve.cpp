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
    abs2 () const noexcept {
        return x * x + y * y;
    }

    T
    abs () const noexcept {
        return std::sqrt (abs2 ());
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

template <typename T, typename U>
auto
dist (const Vector <T>& p1, const Vector<T>& p2) {
    auto p {p1};
    p -= p2;
    return p.abs ();
}

template <typename T>
T
abs2 (const Vector <T>& p) noexcept {
    return p.abs2 ();
}

template <typename T>
T
abs (const Vector <T>& p) noexcept {
    return p.abs ();
}

template <typename T>
struct Line {
    T a, b, c;

    Line (T a = 0, T b = 0, T c = 0) :
        a {a}, b {b}, c {c}
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

} // namespace gtr

int main () {
    std::ios_base::sync_with_stdio (false);

    gtr::Triangle <long> triangle;
    std::cin >> triangle;
    do {
        gtr::Vector <long> point;
        std::cin >> point;
        if (std::cin.fail ()) {
            break;
        }

        auto in_triang = gtr::vector_in_triangle (triangle, point);
        std::cout << (in_triang ? "YES\n" : "NO\n");
    } while (true);
}