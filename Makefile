CC = g++
CXXFLAGS = -O3 -mcx16 -march=native -std=c++17 -Wall -Wextra -Werror -fcilkplus -DCILK -fpic

all: paramsort seqmsort

paramsort: paramsort.cpp
	$(CC) $(CXXFLAGS) paramsort.cpp -o paramsort

seqmsort: seqmsort.cpp
	$(CC) $(CXXFLAGS) seqmsort.cpp -o seqmsort

clean:
	rm paramsort

.PHONY: clean
