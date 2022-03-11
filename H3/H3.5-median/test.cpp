#include "solve.hpp"
#include "../../libs/other_func.hpp"
#include <gtest/gtest.h>
#include <fstream>

TEST (SOLVE_NATIVE, STATIC) {
    median_array_native <int> arr;
    
    arr.add (5);
    arr.add (6);
    ASSERT_EQ (arr.median (), 5);

    arr.add (-4);
    ASSERT_EQ (arr.median (), 5);
}

TEST (SOLVE, RANDOM) {
    
}