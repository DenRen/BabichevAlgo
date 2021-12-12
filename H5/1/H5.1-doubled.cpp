#include <iostream>
#include <cstdint>
#include <vector>

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

template <typename HashT>
std::vector <HashT>
gen_ptab (std::size_t size,
          const HashT p)
{
    std::vector <HashT> ptab (size);
    HashT mul = 1;
    
    for (auto& item : ptab) {
        item = mul;
        mul *= p;
    }

    return ptab;
}

template <typename HashT = uint32_t>
HashT hash (const std::string& str,
            std::size_t begin,
            std::size_t end,
            const std::vector <HashT>& ptab)
{
    HashT hash = 0;
    for (std::size_t i = begin; i < end; ++i) {
        hash += str[i] * ptab[i - begin];
    }

    return hash;
}

template <typename HashT>
std::vector <HashT>
calc_hash_table (const std::string str,
                 const std::vector <HashT>& ptab)
{
    const auto size = str.size ();

    std::vector <HashT> htab (size + 1);
    for (std::size_t i = 0; i < size; ++i) {
        htab[i + 1] = htab[i] + str[i] * ptab[i];
    }

    return htab;
}

template <typename HashT = uint32_t>
std::string solve (std::string src) {
    const auto src_size = src.size ();

    const HashT size_ptab = 260;
    const HashT p = 263;

    auto ptab = gen_ptab <HashT> (size_ptab, p);
    auto htab = calc_hash_table (src, ptab);
    
    std::string res;
    bool previous_is_find = false;

    for (std::size_t size = 1; size < src_size; ++size) {

        std::size_t pos = 0;
        for (; pos <= src_size - size; ++pos) {
            HashT pattern_hash = htab[pos + size] - htab[pos];
            pattern_hash *= p;
    
            for (std::size_t i = pos + 1; i <= src_size - size; ++i) {
                auto cur_hash = htab[i + size] - htab[i];

                if (cur_hash == pattern_hash) {
                    if (src.compare (pos, size, src, i, size) == 0) {
                        previous_is_find = true;
                        res = src.substr (pos, size);

                        pos = INT64_MAX - 1;
                        break;
                    }
                }

                pattern_hash *= p;
            }
        }
        
        if (pos != INT64_MAX && previous_is_find == true) {
            return res;
        }
    }

    return res;
}

int main () {
    std::string src;
    std::cin >> src;

    std::cout << solve (src);
}
