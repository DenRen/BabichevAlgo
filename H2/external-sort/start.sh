#/bin/bash

clang++ -std=c++17 -O3 main.cpp solve4.cpp
rm output.txt
sleep 1
/bin/time -f "%e" ./a.out
diff -q output.txt test_input_sovle.txt