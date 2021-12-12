#include <gtest/gtest.h>
#include <random>
#include <sstream>

#include "other_func.hpp"
#include "solve.hpp"

// g++ test.cpp -lgtest -lgtest_main -lpthread -Werror -std=c++17 -O3 && clear && ./a.out

TEST (H5_1, 1) {
    EXPECT_EQ ("",  solve ("abcdef"))      << std::endl;
    EXPECT_EQ ("ab", solve ("abcabef")) << std::endl;
    EXPECT_EQ ("ab", solve ("cabcloabefioef")) << std::endl;
    EXPECT_EQ ("ba", solve ("bbaaba")) << std::endl;
    EXPECT_EQ ("tabc", solve ("ababfgfgfgbetabcfioeftabc")) << std::endl;
    EXPECT_EQ ("gf", solve ("gugigogqgsfdabcapbefioezf_gfgfop")) << std::endl;
    EXPECT_EQ ("fffff", solve ("ababfgfffffgfgbetabcfioeftfabcfffff")) << std::endl;
    EXPECT_EQ ("vvvv", solve ("qqqaqbabvvvvfgfegfgbetabcfioeftavvvvbc")) << std::endl;
    EXPECT_EQ ("tabc", solve ("ababfgfgfgbetabcftabc")) << std::endl;
}

TEST (KARP_RABIN, 1) {
    const unsigned size_ptab = 260;
    const unsigned p = 301;
   
    std::vector <unsigned> ptab = gen_ptab <unsigned> (size_ptab, p);
    
    ASSERT_EQ (karp_rabin ("abc", "dbccabcd", ptab), 4);
    ASSERT_EQ (karp_rabin ("abc", "dbccffwabcd", ptab), 7);
    ASSERT_EQ (karp_rabin ("db", "dbccabcd", ptab), 0);
    ASSERT_EQ (karp_rabin ("db", "qbccabcdbb", ptab), 7);
    ASSERT_EQ (karp_rabin ("db", "qbccabcdb", ptab), 7);
}

template <typename RandT>
std::string
get_random_string (std::size_t size,
                   RandT& rand,
                   char min_symb = 'a',
                   char max_symb = 'z')
{
    std::string str;
    str.reserve (size);
    for (std::size_t i = 0; i < size; ++i) {
        char symb = min_symb +  rand () % (max_symb - min_symb);
        str.push_back (symb);
    }

    return str;
}

TEST (H5_1, AUTO) {
    std::random_device rd;
    std::minstd_rand0 mersenne (rd ());
    
    const std::size_t min_src_size = 3;
    const std::size_t max_src_size = 170;

    const std::size_t ptrn_min_size = 2;
    const std::size_t ptrn_max_size = 90;

    const std::size_t repeat = 3;
    
    for (std::size_t irepeat = 0; irepeat < repeat; ++irepeat)
    for (std::size_t src_size  = min_src_size;  src_size  <= max_src_size;  ++src_size) {
    for (std::size_t ptrn_size = ptrn_min_size; ptrn_size <= ptrn_max_size; ++ptrn_size) {
        std::string pattern = get_random_string (ptrn_size, mersenne);
        const std::size_t num_patterns = 2 + mersenne () % 5;

        std::stringstream stream;

        for (std::size_t num_pattern = 0; num_pattern < num_patterns;) {
            const std::size_t size = mersenne () % src_size;
            std::string tmp = get_random_string (size, mersenne);

            if (tmp.find (pattern) == std::string::npos &&
                pattern.find (tmp) == std::string::npos) {
                stream << tmp;
                stream << pattern;
                ++num_pattern;
            }
        }

        std::string src = stream.str ();
        
        std::string calc_res = solve (src);
        if (pattern != calc_res) {

            std::size_t pos = 0;
            for (int i = 0; i < 2; ++i) {
                pos = src.find (calc_res, pos);
                if (pos == std::string::npos) {
                    ASSERT_TRUE (false)
                        << "pattern: " << pattern << std::endl
                        << "src: " << src << std::endl
                        << "find res: " << calc_res << std::endl;
                }
                ++pos;
            }
        }
    }
        if ((src_size - min_src_size) % 10 == 0)
            std::cout << irepeat << "/" << repeat - 1 << " "
                      << src_size - min_src_size << "/"
                      << max_src_size - min_src_size << std::endl;
    }
}
