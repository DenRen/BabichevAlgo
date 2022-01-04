#include <iostream>
#include <algorithm>
#include <vector>

struct cov_t {
    std::size_t left, right;

    cov_t () :
        left (0), right (0)
    {}

    std::size_t sum () const noexcept {
        return left + right;
    }

    bool operator > (const cov_t& rhs) const noexcept {
        return sum () > rhs.sum ();
    }
    
    bool operator < (const cov_t& rhs) const noexcept {
        return sum () < rhs.sum ();
    }

    bool operator == (const cov_t& rhs) const noexcept {
        return left == rhs.left && right == rhs.right;
    }
};

std::ostream&
operator << (std::ostream& os, const cov_t& cov) {
    return os << "{" << cov.left << "+" << cov.right << "}";
}

// V - sorted
template <typename T>
std::vector <cov_t>
coverage (T L, std::vector <T>& V) {
    std::vector <cov_t> ret (V.size ());

    for (std::size_t i = 0; i < V.size (); ++i) {
        cov_t res;
        const T ref = V[i];
        for (std::size_t l = i - 1; l + 1 != 0; --l) {
            if (V[l] + L >= ref) {
                ++res.left;
            } else {
                break;
            }
        }

        for (std::size_t r = i + 1; r != V.size (); ++r) {
            if (ref + L >= V[r]) {
                ++res.right;
            } else {
                break;
            }
        }

        ret[i] = res;
    }

    return ret;
}

template <typename T>
std::size_t
dance (T L, std::vector <T>& V) {
    std::sort (V.begin (), V.end ());
    std::vector <cov_t> cov = coverage (L, V);
    
    const auto num_zero = std::count (cov.cbegin (), cov.cend (), cov_t {});
    std::size_t res = num_zero;

    for (std::size_t num_alive = V.size () - num_zero; num_alive != 0; ) {
        // Find max
        const auto cov_max_iter = std::max_element (cov.begin (), cov.end ());
        num_alive -= cov_max_iter->sum () + 1;

        // Correction left part
        T ctr = 0;
        const auto left_begin = cov_max_iter - cov_max_iter->left;
        const auto left_correction_begin = left_begin - left_begin->left;
        for (auto l = left_begin - 1; l + 1 != left_correction_begin; --l) {
            l->right = ctr++;
        }

        // Correction right part
        ctr = 0;
        const auto right_begin = cov_max_iter + cov_max_iter->right;
        const auto right_correction_begin = right_begin + right_begin->right;
        for (auto r = right_begin + 1; r != right_correction_begin + 1; ++r) {
            r->left = ctr++;
        }

        // Fill zero center
        std::fill (cov_max_iter - cov_max_iter->left,
                   cov_max_iter + cov_max_iter->right + 1, cov_t {});
        
        // All cov_max_iter->sum () + 1 points dance in one point
        ++res;
    }
    
    return res;
}

int main () {
    int L = 0;
    std::size_t N = 0;
    std::cin >> L >> N;

    std::vector <int> V (N);
    for (auto& v : V) {
        std::cin >> v;
    }

    std::cout << dance (L, V) << std::endl;
}