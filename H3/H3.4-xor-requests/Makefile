DEBUG_FLAGS = -g3 -O0
GFLAGS = -std=c++17
GTEST_FLAGS = -lgtest -lgtest_main -lpthread

all: main test

main: main.cpp
	g++ main.cpp ${GFLAGS} -o main.out

test_debug:
	g++ test.cpp ${GFLAGS} ${GTEST_FLAGS} ${DEBUG_FLAGS} -o test.out

test: test.cpp
	g++ test.cpp ${GFLAGS} ${GTEST_FLAGS} -O3 -o test.out

clean:
	rm *.out *.o