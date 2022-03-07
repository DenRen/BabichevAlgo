#!/bin/bash

./a.out < 1 > .1r
diff .1r 1_solve

./a.out < 2 > .2r
diff .2r 2_solve

./a.out < 3 > .3r
diff .3r 3_solve

