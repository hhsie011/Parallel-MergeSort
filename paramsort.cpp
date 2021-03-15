#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include "get_time.h"

#include <cmath>
#include <ctime>

using namespace std;

int th = 128;

void merge(int* arr, int start, int end) {
    int mid = (start + end) / 2;
    int* left = new int[mid - start + 1];
    int* right = new int[end - mid + 1];
    int lptr = start;
    int rptr = mid + 1;
    int ptr = start;

    copy(arr + start, arr + mid + 1, left);
    copy(arr + mid + 1, arr + end + 1, right);

    lptr = 0;
    rptr = 0;

    while (lptr < mid - start + 1 && rptr < end - mid) {
	arr[ptr++] = (left[lptr] <= right[rptr])? left[lptr++] : right[rptr++];
    }

    while (lptr < mid - start + 1) {
	arr[ptr++] = left[lptr++];
    }

    while (rptr < end - mid) {
	arr[ptr++] = right[rptr++];
    }

    delete[] left;
    delete[] right;
    
    return;
}

void paramsort(int* arr, int start, int end) {
    if (start == end)
	return;
    int mid = (start + end) / 2;
    if (end - start < th) {
	paramsort(arr, start, mid);
	paramsort(arr, mid + 1, end);
    }
    else {
        cilk_spawn paramsort(arr, start, mid);
        paramsort(arr, mid + 1, end);
        cilk_sync;
    }
    merge(arr, start, end);
    return;
}


int main(int argc, char** argv) {
    if (argc != 2)
	cout << "Error: input value missing" << endl;

    srand((unsigned) time(0));
    int n = atoi(argv[1]);
    int* arr = new int[n];

    for (int i = 0; i < n; ++i) {
	arr[i] = 1 + (rand() % (n * 2));
    }

    timer t; t.start();
    paramsort(arr, 0, n-1);
    t.stop(); cout << "time: " << t.get_total() << endl;

    // for (int i = 0; i < n; ++i) {
	// cout << arr[i] << endl;
    // }

    delete[] arr;

    return 0;
}
