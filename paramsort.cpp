#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cmath>
#include <ctime>

using namespace std;

void paramerge(int* arr, int start, int end) {
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
	/*if (left[lptr] <= right[rptr]) {
	    arr[ptr++] = left[lptr++];
	}
	else {
	    arr[ptr++] = right[rptr++];
	}*/
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
    cilk_spawn paramsort(arr, start, mid);
    paramsort(arr, mid + 1, end);
    cilk_sync;
    paramerge(arr, start, end);
    return;
}


int main(int argc, char** argv) {
    if (argc != 2)
	cout << "Error: value missing" << endl;
    srand((unsigned) time(0));
    int n = atoi(argv[1]);
    /*double sum = 0;
	
    for (int i = 1; i <= n; ++i) {
	sum += pow(sin(i/static_cast<double>(n)), 5);
    } 
    cout << "Sequential sum: " << sum << endl;

    sum = 0;
    cilk_for (int i = 1; i <= n; ++i) {
	sum += pow(sin(i/static_cast<double>(n)), 5);
    }
    cout << "Parallel sum: " << sum << endl;*/
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
	arr[i] = 1 + (rand() % 100);
    }
    paramsort(arr, 0, n-1);
    for (int i = 0; i < n; ++i) {
	cout << arr[i] << endl;
    }
    delete[] arr;
    return 0;
}
