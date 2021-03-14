CC = g++
CXXFLAGS = -O3 -mcx16 -march=native -std=c++17 -Wall -Wextra -Werror -fcilkplus -DCILK -fpic

all: paramsort

test: paramsort.cpp
	$(CC) $(CXXFLAGS) paramsort.cpp -o paramsort

clean:
	rm paramsort

.PHONY: clean
