#include <gtest/gtest.h>
#include <random>

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
