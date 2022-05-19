#pragma once

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
#include <complex>
#include <type_traits>
#include <exception>

// g++ -DHOST -std=c++17 main.cpp

// #define NDEBUG
#define HOST

#ifdef HOST
    #include "../../libs/print_lib.hpp"
    #define DUMP(obj) //std::cerr << #obj ": " << obj << '\n'
#else
    #define DUMP(obj)
#endif

namespace gtr {
// Compare

// template <typename T, typename U>
// std::enable_if_t <std::is_floating_point_v <T> ||
//                   std::is_floating_point_v <U>, bool>


template <typename T, typename U, std::enable_if_t <std::is_floating_point_v <T> ||
                  std::is_floating_point_v <U>>* = nullptr>
bool
is_equal (T a, U b) {
    // constexpr auto eps_a = 100 * std::numeric_limits <T>::epsilon ();
    // constexpr auto eps_b = 100 * std::numeric_limits <U>::epsilon ();
    constexpr auto eps_a = 1e-4;
    constexpr auto eps_b = 1e-4;

    using X = decltype (a - b);
    constexpr auto eps = std::max <X> (eps_a, eps_b);

    return std::fabs (a - b) < eps;
}

template <typename T, typename U, std::enable_if_t <std::is_integral_v <T> &&
                  std::is_integral_v <U>>* = nullptr>
bool
is_equal (T a, U b) {
    return a == b;
}

template <typename T, typename U>
std::enable_if_t <std::is_floating_point_v <T> ||
                  std::is_floating_point_v <U>, bool>
is_less (T a, U b) {
    return !is_equal (a, b) && a < b;
}

template <typename T, typename U>
std::enable_if_t <std::is_floating_point_v <T> ||
                  std::is_floating_point_v <U>, bool>
is_less_eq (T a, U b) {
    return is_equal (a, b) || a < b;
}


template <typename T, typename U>
std::enable_if_t <std::is_integral_v <T> &&
                  std::is_integral_v <U>, bool>
is_less (T a, U b) {
    return a < b;
}

template <typename T, typename U>
std::enable_if_t <std::is_integral_v <T> &&
                  std::is_integral_v <U>, bool>
is_less_eq (T a, U b) {
    return a <= b;
}

template <typename T, typename U>
bool
is_greater (T a, U b) {
    return !is_less_eq (a, b);
}

template <typename T, typename U>
bool
is_greater_eq (T a, U b) {
    return !is_less (a, b);
}

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

    Vector&
    reflect_x () noexcept {
        x = -x;
        return *this;
    }

    Vector&
    reflect_y () noexcept {
        y = -y;
        return *this;
    }

    Vector&
    transpose () noexcept {
        std::swap (x, y);
        return *this;
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
    auto p = p1 - p2;
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
bool
is_equal (const Vector <T>& lhs,
          const Vector <T>& rhs) noexcept {
    return is_equal (lhs.x, rhs.x) && is_equal (lhs.y, rhs.y);
}

template <typename T>
struct Line {
    T a, b, c;

    Line (T a = 0, T b = 0, T c = 0) :
        a {a}, b {b}, c {c}
    {}

    inline static T
    calc_a (const Vector <T>& p,
            const Vector <T>& q) {
        return q.y - p.y;
    }

    inline static T
    calc_b (const Vector <T>& p,
            const Vector <T>& q) {
        return p.x - q.x;
    }

    inline static T
    calc_c (const Vector <T>& p,
            const Vector <T>& q) {
        return q.x * p.y - p.x * q.y;
    }

    // TODO: вероятно неправильно строит прямую
    Line (const Vector <T>& p,
          const Vector <T>& q) :
          a {calc_a (p, q)},
          b {calc_b (p, q)},
          c {calc_c (p, q)}
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
        return {b, -a}; // Don't change!
    }

    Vector <T>
    co_dir () const {
        return unnorm_co_dir () / std::sqrt (a * a + b * b);
    }

    T
    operator () (T x, T y) const noexcept {
        return a * x + b * y + c;
    }

    Line&
    transpose () noexcept {
        std::swap (a, b);
        return *this;
    }
};

template <typename T>
Vector <T>
intersect (const Line <T>& line1,
           const Line <T>& line2) {
    auto det_denom = line1.a * line2.b - line1.b * line2.a;
    auto x = (line1.b * line2.c - line1.c * line2.b) / det_denom;
    auto y = (line1.c * line2.a - line1.a * line2.c) / det_denom;
    return {x, y};
}

template <typename T>
std::ostream&
operator << (std::ostream& os,
             const Line <T>& line) {
    return os << '{' << line.a << ',' << line.b << ',' << line.c << '}';
}

template <typename T>
std::istream&
operator >> (std::istream& is,
             const Line <T>& line) {
    return is >> line.a >> line.b >> line.c;
}

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

template <typename T>
struct LineSegment {
    gtr::Vector <T> p, q;

    LineSegment () = default;

    LineSegment (const LineSegment&) = default;
    LineSegment (LineSegment&&) = default;

    LineSegment&
    operator = (const LineSegment&) = default;
    LineSegment&
    operator = (LineSegment&&) = default;

    LineSegment (const gtr::Vector <T>& p,
                 const gtr::Vector <T>& q) :
        p (p), q (q)
    {}

    void
    sort_x () noexcept {
        if (p.x > q.x) {
            std::swap (p, q);
        }
    }

    void
    sort () noexcept {
        if (is_less (p.x, q.x) || (is_equal (p.x, q.x) && is_less_eq (p.y, q.y))) {
            return;
        }
        std::swap (p, q);
    }

    bool
    is_sorted_x () const noexcept {
        return p.x <= q.x;
    }
    bool
    is_sorted () const noexcept {
        return is_sorted_x () || (p.x == q.x && p.y <= q.y);
    }

    // LineSegment should be sorted
    bool
    is_collinear_point_belong (const Vector <T>& p)
    #ifdef NDEBUG
        noexcept
    #endif
    {
        #ifndef NDEBUG
            if (!is_sorted_x ()) {
                throw std::runtime_error ("LineSegment is not sorted!");
            }
        #endif

        return p.x >= this->p.x && p.x <= this->q.x &&
               (this->p.y <= this->q.y ? p.y >= this->p.y && p.y <= this->q.y :
                                         p.y >= this->q.y && p.y <= this->p.y);
    }

    LineSegment&
    transpose () noexcept {
        p.transpose ();
        q.transpose ();
        return *this;
    }

    Vector <T>
    unnorm_co_dir () const noexcept {
        return q - p;
    }
};

template <typename T>
bool
is_collinear (const Vector <T>& p,
              const Vector <T>& q) noexcept {
    return is_equal (p.x * q.y, p.y * q.x);
}

template <typename T>
bool
is_intersect (const Vector <T>& point,
              const Line <T>& line) noexcept {
    return is_equal (line (point.x, point.y), 0);
}

template <typename T>
std::istream&
operator >> (std::istream& is,
             LineSegment <T>& ls) {
    return is >> ls.p >> ls.q;
}

template <typename T>
std::ostream&
operator << (std::ostream& os,
             const LineSegment <T>& ls) {
    return os << '[' << ls.p << ',' << ls.q << ']';
}

template <typename T>
bool
is_intersect (LineSegment <T>& ls1,
              LineSegment <T>& ls2) noexcept {
    ls1.sort_x ();
    ls2.sort_x ();

    Line <T> line1 {ls1.p, ls1.q}, line2 {ls2.p, ls2.q};

    auto co_dir_1 = ls1.unnorm_co_dir ();
    auto co_dir_2 = ls2.unnorm_co_dir ();
    if (is_collinear (co_dir_1, co_dir_2)) {    // Parallel
        if (!is_equal (line1.a * line2.c, line2.a * line1.c) || // ||, //, =
            !is_equal (line1.b * line2.c, line2.b * line1.c)) {
            return false;
        }

        return ls1.is_collinear_point_belong (ls2.p) ||
               ls1.is_collinear_point_belong (ls2.q) ||
               ls2.is_collinear_point_belong (ls1.p) ||
               ls2.is_collinear_point_belong (ls1.q);
    } else {
        auto p = intersect (line1, line2);
        return ls1.is_collinear_point_belong (p) &&
               ls2.is_collinear_point_belong (p);
    }
}

template <typename T>
std::vector <Vector <T>>
build_convex_hull_graham (std::vector <Vector <T>>& ps) {
    if (ps.size () <= 1) {
        return {};
    }

    auto P = *std::min_element (ps.cbegin (), ps.cend (), [] (const auto& lhs, const auto& rhs) {
        return gtr::is_less (lhs.y, rhs.y) ||
               (gtr::is_equal (lhs.y, rhs.y) && gtr::is_greater (lhs.x, rhs.x));
    });
    std::vector <Vector <T>> hull {P};

    auto cmp = [P] (const Vector <T> & lhs, const Vector <T>& rhs) {
            auto lhs_ = lhs - P, rhs_ = rhs - P;
            auto cross = lhs_ ^ rhs_;
            return is_greater (cross, 0) ||
                   (is_equal (cross, 0) && is_greater (lhs_.abs_square (), rhs_.abs_square ()));
    };

    std::sort (ps.begin (), ps.end (), cmp);

    DUMP ("begin");
    DUMP (P);
    // for (const auto& p : ps) {
    //     std::cerr << p << '\n';
    // }

    auto size = ps.size ();
    for (std::size_t i = 0; i + 1 < size; ++i) {
        const auto& w = ps[i];
        DUMP (w);

        while (hull.size () >= 2) {
            auto j = hull.size () - 1;
            
            auto w_p = P - w;
            auto p2_p1 = hull[j] - hull[j - 1];
            auto p1_w = w - hull[j];
            
            auto cross = p2_p1 ^ p1_w;
            auto cross_in_w = w_p ^ p1_w;

            DUMP (cross);
            if (cross > 0) {
                if (!is_equal (cross_in_w, 0)) {
                    hull.push_back (w);
                }
                break;
            } else if (is_equal (cross, 0)) {
                if (!is_equal (cross_in_w, 0)) {
                    hull[j] = w;
                }
                break;
            } else {
                hull.pop_back ();
            }
        }

        if (hull.size () < 2) {
            DUMP ("size < 2 => push w");
            hull.push_back (w);
        }
    }

    // if (is_equal (hull[hull.size () - 1], P)) {
    //     hull.pop_back ();
    // }

    // for (const auto& p : hull) {
    //     std::cerr << "h: " << p << '\n';
    // }

    return hull;
}

// Native
template <typename T>
bool
is_convex_hull (const std::vector <Vector <T>>& ch,
                const std::vector <Vector <T>>& ps) {
    if (ch.size () > ps.size ()) {
        return false;
    }

    // for (const auto& p : ps) {
    //     std::cout << p << std::endl;
    // }

    for (std::size_t i = 0; i < ch.size (); ++i) {
        auto i_next = (i + 1) % ch.size ();
        const auto& p = ch[i], &q = ch[i_next];

        Line <T> line {p, q};

        // Check on points on the one side
        int up = 0, down = 0;        
        for (std::size_t j = 0; j < ps.size (); ++j) {
            auto res = line (ps[j].x, ps[j].y);
            DUMP (res);
            if (is_equal (res, 0)) {
                LineSegment <T> ls {p, q};
                ls.sort ();
                if (ls.is_collinear_point_belong (ps[j]) == false) {
                    DUMP (ls);
                    DUMP (ps[j]);
                    return false;
                }
                continue;
            }

            if (res < 0) {
                ++down;
            } else {
                ++up;
            }
        }

        if (up != 0 && down != 0) {
            return false;
        }
    }

    // Check on collinear
    if (ch.size () > 2) {
        for (std::size_t i = 0; i < ch.size (); ++i) {
            const auto& p = ch[i];
            const auto& q = ch[(i + 1) % ch.size ()];
            const auto& l = ch[(i + 2) % ch.size ()];

            if (gtr::is_equal ((p - q) ^ (q - l), 0)) {
                return false;
            }
        }
    }

    return true;
}

template <typename T>
T
perimeter (const std::vector <Vector <T>>& ps) noexcept {
    if (ps.size () <= 1) {
        return 0;
    }

    T p = dist (ps[ps.size () - 1], ps[0]);
    for (std::size_t i = 1; i < ps.size (); ++i) {
        p += dist (ps[i - 1], ps[i]);
    }

    return p;
}

} // namespace gtr