#include <gtest/gtest.h>
#include <chrono>
#include "geoma.hpp"
#include "../../libs/other_func.hpp"

// g++ test.cpp -lgtest -lgtest_main -pthread -std=c++17 -O3 -o test.out

TEST (VECTOR_IN_TRIANGLE, STATIC) {
    gtr::Triangle <int> tr {{0, 0}, {0, 10}, {10, 10}};
    std::vector <std::pair <gtr::Vector <int>, bool>> points = {
        {{0, 0}, true},
        {{0, 1}, true},
        {{1, 1}, true},
        {{1, 2}, true},
        {{1, 3}, true},
        {{5, 6}, true},
        {{6, 5}, false},
        {{10, 10}, true},
        {{11, 9}, false}
    };

    for (const auto&[point, state] : points) {
        ASSERT_EQ (gtr::vector_in_triangle (tr, point), state)
            << point;
    }
}

TEST (IS_EQ_LESS_GREATER, STATIC) {
    auto test = [] (auto a, auto b) {
        ASSERT_TRUE  (gtr::is_equal (a, a));
        ASSERT_FALSE (gtr::is_equal (a, b));

        ASSERT_TRUE  (gtr::is_less (a, b));
        ASSERT_FALSE (gtr::is_less (b, a));
        ASSERT_FALSE (gtr::is_less (a, a));

        ASSERT_TRUE  (gtr::is_less_eq (a, a));
        ASSERT_TRUE  (gtr::is_less_eq (a, b));
        ASSERT_FALSE (gtr::is_less_eq (b, a));

        ASSERT_TRUE  (gtr::is_greater (b, a));
        ASSERT_FALSE (gtr::is_greater (a, b));
        ASSERT_FALSE (gtr::is_greater (a, a));

        ASSERT_TRUE  (gtr::is_greater_eq (a, a));
        ASSERT_TRUE  (gtr::is_greater_eq (b, a));
        ASSERT_FALSE (gtr::is_greater_eq (a, b));
    };

    test (1, 2);

    test (1.0, 2);
    test (1, 2.0);
    test (1.0, 2.0);

    test (1.0f, 2);
    test (1, 2.0f);
    test (1.0f, 2.0f);

    test (1.0f, 1.00002);
    test (1.0f, 1.00009);
}

TEST (IS_INTERSECT_LS_LS, STATIC) {
    // {
    //     gtr::LineSegment <double> ls1 {{1, 1}, {7, 4}},
    //                               ls2 {{5, 3}, {3, 2}};
    //     EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    // }
    // {
    //     gtr::LineSegment <double> ls1 {{-1, -1}, {1, 1}},
    //                               ls2 {{-2, -2}, {2, 2}};
    //     EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    // }
    // {
    //     gtr::LineSegment <double> ls1 {{-1, -1}, {-2, -2}},
    //                               ls2 {{+2, +2}, {-0.999, -0.999}};
    //     EXPECT_FALSE (gtr::is_intersect (ls1, ls2));
    // }
    // {
    //     gtr::LineSegment <double> ls1 {{1, 4}, {3, 5}},
    //                               ls2 {{5, 6}, {7, 7}};
    //     EXPECT_FALSE (gtr::is_intersect (ls1, ls2));
    // }
    // {
    //     gtr::LineSegment <double> ls1 {{-1, -1}, {0, 0}},
    //                               ls2 {{0.1, 0.1}, {3, 2}};
    //     EXPECT_FALSE (gtr::is_intersect (ls1, ls2));
    // }
    // {
    //     gtr::LineSegment <double> ls1 {{-1, -10}, {5, 11.76}},
    //                               ls2 {{5, -10}, {-20, 30}};
    //     EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    // }
    // {
    //     gtr::LineSegment <double> ls1 {{-1, -1}, {-0.1, -0.1}},
    //                               ls2 {{-0.05, -0.05}, {20, 30}};
    //     EXPECT_FALSE (gtr::is_intersect (ls1, ls2));
    // }
    // {
    //     gtr::LineSegment <double> ls1 {{-3, 0}, {6, 3}},
    //                               ls2 {{3, 2}, {5, 0}};
    //     EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    // }
    // {
    //     gtr::LineSegment <double> ls1 {{-3e5, 0}, {6e5, 3e5}},
    //                               ls2 {{3e5, 2e5}, {5e5, 0}};
    //     EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    // }
    // {
    //     gtr::LineSegment <double> ls1 {{-3, 0}, {6, 3}},
    //                               ls2 {{4, 2}, {5, 0}};
    //     EXPECT_FALSE (gtr::is_intersect (ls1, ls2));
    // }
    // {
    //     gtr::LineSegment <double> ls1 {{-3, 0}, {3, 2}},
    //                               ls2 {{3, 2}, {5, 0}};
    //     EXPECT_TRUE (gtr::is_intersect (ls1, ls2));
    // }
}

TEST (IS_INTERSECT_LS_LS_2, STATIC) {
    auto test_swap = [] (gtr::Vector <double> p1, gtr::Vector <double> q1,
                         gtr::Vector <double> p2, gtr::Vector <double> q2,
                         bool& res) {
        std::vector <bool> ress;

        {
            gtr::LineSegment <double> ls1 {p1, q1}, ls2 {p2, q2};
            ress.push_back (gtr::is_intersect <double> (ls1, ls2));
            ress.push_back (gtr::is_intersect <double> (ls2, ls1));
        }
        std::swap (p1, q1);
        std::swap (p2, q2);
        {
            gtr::LineSegment <double> ls1 {p1, q1}, ls2 {p2, q2};
            ress.push_back (gtr::is_intersect <double> (ls1, ls2));
            ress.push_back (gtr::is_intersect <double> (ls2, ls1));
        }

        for (std::size_t i = 1; i < ress.size (); ++i) {
            ASSERT_EQ (ress[i - 1], ress[i]) << p1 << q1 << p2 << q2;
        }

        res = ress[0];
    };

    auto test_move = [&] (gtr::Vector <double> p1, gtr::Vector <double> q1,
                          gtr::Vector <double> p2, gtr::Vector <double> q2,
                          bool& res) {
        std::vector <bool> ress;

        for (double dx = -1714; dx <= 1714; dx += 13) {
            for (double dy = -1714; dy <= 1714; dy += 17) {
                gtr::Vector d {dx, dy};
                auto p1_ = p1 + d, q1_ = q1 + d;
                auto p2_ = p2 + d, q2_ = q2 + d;
                bool res_ = false;
                test_swap (p1_, q1_, p2_, q2_, res_);
                ress.push_back (res_);
            }
        }

        for (std::size_t i = 1; i < ress.size (); ++i) {
            ASSERT_EQ (ress[i - 1], ress[i]) << p1 << q1 << p2 << q2;
        }

        res = ress[0];
    };

    auto test = [&] (gtr::Vector <double> p1, gtr::Vector <double> q1,
                     gtr::Vector <double> p2, gtr::Vector <double> q2,
                     bool& res) {
        std::vector <bool> ress;
        bool res_ = false;

        test_move (p1, q1, p2, q2, res_);//
        ress.push_back (res_);
        {
            auto p1_ {p1}, p2_ {p2}, q1_ {q1}, q2_ {q2};
            p1_.reflect_x (); p2_.reflect_x ();
            q1_.reflect_x (); q2_.reflect_x ();
            test_move (p1_, q1_, p2_, q2_, res_);//
            ress.push_back (res_);
        }
        {
            auto p1_ {p1}, p2_ {p2}, q1_ {q1}, q2_ {q2};
            p1_.reflect_y (); p2_.reflect_y ();
            q1_.reflect_y (); q2_.reflect_y ();
            test_move (p1_, q1_, p2_, q2_, res_);
            ress.push_back (res_);
        }
        {
            auto p1_ {p1}, p2_ {p2}, q1_ {q1}, q2_ {q2};
            p1_.reflect_x (); p2_.reflect_x ();
            q1_.reflect_x (); q2_.reflect_x ();
            p1_.reflect_y (); p2_.reflect_y ();
            q1_.reflect_y (); q2_.reflect_y ();
            test_move (p1_, q1_, p2_, q2_, res_);
            ress.push_back (res_);
        }

        for (std::size_t i = 1; i < ress.size (); ++i) {
            ASSERT_EQ (ress[i - 1], ress[i]) << p1 << q1 << p2 << q2;
        }

        res = ress[0];
    };

    auto test_true = [&] (gtr::Vector <double> p1, gtr::Vector <double> q1,
                          gtr::Vector <double> p2, gtr::Vector <double> q2) {
        bool res = false;
        test (p1, q1, p2, q2, res);
        ASSERT_TRUE (res) << p1 << q1 << p2 << q2;
    };
    auto test_false = [&] (gtr::Vector <double> p1, gtr::Vector <double> q1,
                           gtr::Vector <double> p2, gtr::Vector <double> q2) {
        bool res = false;
        test (p1, q1, p2, q2, res);
        ASSERT_FALSE (res) << p1 << q1 << p2 << q2;
    };

    // Topology 1
    test_true ({9, 4}, {13, 8}, {10, 4}, {12, 8});
    test_true ({9, 4}, {13, 8}, {11, 4}, {10, 8});
    test_true ({9, 4}, {13, 8}, {10, 4}, {12, 8});
    test_true ({9, 4}, {13, 8}, { 4, 9}, {14, 4});

    test_false ({9, 4}, {10, 4}, {13, 8}, {12, 8});
    

    test_false ({9, 4}, {11, 4}, {13, 8}, {10, 8});
    test_false ({1, 3}, {1, 6}, {2, 4}, {2, 5});


    test_false ({9, 4}, {10, 4}, {13, 8}, {12, 8});
    test_false ({9, 4}, { 4, 9}, {13, 8}, {14, 4});

    // Topology 2
    test_false ({0, 0}, {1, 1}, {2, 2}, {3, 3});
    test_false ({5, 2}, {6, 1}, {6.5, 0.5}, {7, 0});
    test_false ({-1e5-7, -1e5-3}, {0-7, 0-3}, {0.5-7, 0.5-3}, {1e5-7, 1e5-3});

    // Topology 3
    test_false ({2, 2}, {3, 3}, {2, 1}, {4, 3});
    test_false ({2, 2}, {3, 3}, {3, 1}, {4, 2});
    test_false ({2, 1}, {4, 3}, {0, 0}, {1, 1});
    test_false ({-1, 1}, {1, -1}, {4, 6}, {5, 5});
    test_false ({4, 8}, {14, 4}, {5, 8}, {19, 2});
    test_false ({5, 2}, {6, 1}, {4, -2}, {5, -3});
    test_false ({9, 3}, {11, 4}, {11, 3}, {13, 4});
    test_false ({9, 3}, {11, 4}, {11, 3}, {15, 5});

    // Topology 4
    test_true ({0, 0}, {2, 2}, {0.5, 0.5}, {1, 1});
    test_true ({9, 4}, {13, 8}, {10, 5}, {11, 6});
    test_true ({9, 4}, {12, 7}, {10, 5}, {11, 6});
    test_true ({4, 8}, {19, 2}, {9, 6}, {14, 4});

    // Topology 5
    test_true ({0, 0}, {1, 1}, {0.5, 0.5}, {2, 2});
    test_true ({9, 4}, {11, 6}, {10, 5}, {13, 8});
    test_true ({9, 4}, {11, 6}, {10, 5}, {12, 7});
    test_true ({4, 8}, {14, 4}, {9, 6}, {19, 2});

    // Topology 6
    test_true ({0, 0}, {1, 1}, {1, 1}, {2, 2});
    test_true ({9, 4}, {11, 6}, {11, 6}, {13, 8});
    test_true ({9, 4}, {11, 6}, {11, 6}, {12, 7});
    test_true ({4, 8}, {9, 6}, {9, 6}, {19, 2});
    test_true ({2, 1}, {3, 2}, {3, 2}, {4, 3});

    // Topology 7
    for (int i = 10; i <= 20; i += 3) {
        for (int j = -20; j <= 4; j += 3) {
            test_false ({9, 4}, {11, 6}, {(double) i, (double) j}, {12, 6});
        }
    }

    test_false ({2, 1}, {4, 3}, {5, 3}, {7, 1});
    test_false ({2, 1}, {4, 3}, {5, 2}, {6, 1});

    // Topology 8
    test_true ({2, 1}, {4, 3}, {4, 3}, {5, 2});
    test_true ({3, 1}, {4, 2}, {4, 2}, {5, 1});
    test_true ({10, 2}, {11, 1}, {11, 1}, {13, 2});
    for (double i = -1e5; i <= 1e5; i += 3e4) {
        for (double j = -1e5; j <= 1e5; j += 3e4) {
            gtr::Vector <double> p {i, j};
            test_true ({-7898, 461}, p, p, {46516, 89791});
        }
    }

/*
    test_false ({}, {}, {}, {});
    test_true ({}, {}, {}, {});
*/

    // Topology 9
    test_false ({0, 0}, {1, 1}, {2, 2}, {3, 1});
    test_false ({2, 1}, {4, 3}, {4.5, 3.5}, {7, 1});
    test_false ({1, -1}, {-1, 1}, {-3, 3}, {8, 5});
    test_false ({1, 0}, {2, 0}, {0, 3}, {0, 5});
    test_false ({1, 0}, {5, 0}, {2, 5}, {3, 2});

    test_true ({13, 1}, {16, 1}, {15, 2}, {16, 1});
    test_false ({13, 1}, {16, 1}, {15, 2}, {14, 3});

    test_false ({8, 1}, {8, 3}, {9, 2}, {10, 3});
    test_true ({8, 1}, {8, 3}, {8, 1}, {10, 3});

    // Topology 10
    test_true ({16, 8}, {18, 4}, {17, 7}, {16, 6});
    test_true ({4, 8}, {19, 2}, {14, 4}, {170, -79});
    test_true ({0, 0}, {0, 1}, {0, 0.5}, {0.5, 0.5});

    // Topology 11
    test_false ({4, 8}, {19, 2}, {11, 6}, {18, 6});
    test_false ({4, 8}, {19, 2}, {11, 6}, {18, 10});
    test_false ({4, 8}, {19, 2}, {11, 6}, {18, 30});

    // Topology 12
    test_true ({0, 0}, {1, 1}, {0, 0}, {1, 1});
    test_true ({-15, 34657}, {78.01213, 98}, {-15, 34657}, {78.01213, 98});
    test_true ({11, 4}, {10, 8}, {11, 4}, {10, 8});

    // Topology 13
    test_true ({0, 0}, {1, 1}, {0, 0}, {2, 2});
    test_true ({0, 0}, {0, 1}, {0, 0}, {0, 2});
    test_false ({0, 0}, {0, 1}, {0, 2}, {0, 3});
    test_true ({1, 0}, {1, 1}, {1, 0}, {1, 2});
    test_false ({1, 0}, {1, 1}, {1, 2}, {1, 3});
    test_true ({9, 4}, {11, 6}, {9, 4}, {13, 8});
    test_true ({9, 4}, {11, 6}, {9, 4}, {12, 7});
    test_true ({4, 8}, {14, 4}, {4, 8}, {19, 2});

    test_false ({0, 0}, {0, 3}, {0, 4}, {1, 5});
}