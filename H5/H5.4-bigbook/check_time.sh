#!/bin/bash

make clean
make main.out

/bin/time -f "%e" ./main.out < test.txt > r
diff -q r test_ref.txt
ls -lah db.txt

# make tmp_clean
# sleep 1

# /bin/time -f "%e" ./main.out < test_full_err.txt > r
# diff -q r test_full_err_ref.txt
# ls -lah db.txt

make tmp_clean
sleep 1

/bin/time -f "%e" ./main.out < test_small_key.txt > r
diff -q r test_small_key_ref.txt
ls -lah db.txt
ls -lah test_small_key.txt

make tmp_clean