#!/bin/bash

make clean
make main.out

/bin/time -f "%e" ./main.out < test.txt > r
diff -q r test_ref.txt

make tmp_clean
sleep 1

/bin/time -f "%e" ./main.out < test_full_err.txt > r
diff -q r test_full_err_ref.txt
make tmp_clean