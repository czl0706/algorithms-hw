#include <iostream>
using namespace std;

template <typename T>
int _partition(T *arr, int left, int right) {
    T tmp, x = arr[right]; // arr[right] is the pivot
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (arr[j] <= x) {
            i++;
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    tmp = arr[i + 1];
    arr[i + 1] = arr[right];
    arr[right] = tmp;

    return i + 1;
}

template <typename T>
void _determineMedian(T *arr, int left, int right) {
    int mid = (left + right) / 2;
    T tmp;
    // find medium index without swapping
    if (arr[mid] > arr[left]) {
        if (arr[mid] < arr[right]) {
            // left, mid, right
            tmp = arr[mid];
            arr[mid] = arr[right];
            arr[right] = tmp;
        } else if (arr[left] > arr[right]) {
            // right, left, mid
            tmp = arr[left];
            arr[left] = arr[right];
            arr[right] = tmp;
        } else {
            // left, right, mid
        }
    } else {
        if (arr[mid] > arr[right]) {
            // right, mid, left
            tmp = arr[mid];
            arr[mid] = arr[right];
            arr[right] = tmp;
        } else if (arr[left] > arr[right]) {
            // mid, left, right
            tmp = arr[left];
            arr[left] = arr[right];
            arr[right] = tmp;
        } else {
            // mid, right, left
        }
    }
}

template <typename T>
void _insertionsort(T *arr, int begin, int end) {
    // [begin, end]
    for (int i = begin + 1; i <= end; i++) {
        T key = arr[i];
        int j = i - 1;

        while (j >= begin && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }

        arr[j + 1] = key;
    }
}

template <typename T>
void _quicksort(T *arr, int left, int right) {
    if (left >= right) {
        return;
    }
    if (right - left < 16) {
        _insertionsort(arr, left, right);
        return;
    }

    _determineMedian(arr, left, right);

    int pivot = _partition(arr, left, right);
    _quicksort(arr, left, pivot - 1);
    _quicksort(arr, pivot + 1, right);
}

template <typename T>
void quickSort(T *arr, int size) {
    _quicksort(arr, 0, size - 1);
}

#include <assert.h>
#include <array>

#define TEST_SIZE 5000000
std::array<int, TEST_SIZE> arr5, arr6;

int main() {
    srand(time(NULL));

    std::array<int, 5> arr1 {1, 2, 3, 4, 5}, arr2 {4, 5, 3, 2, 1};
    quickSort(arr2.data(), arr2.size());
    assert(arr1 == arr2);

    std::array<char, 5> arr3 {'a', 'b', 'c', 'd', 'e'}, arr4 {'b', 'c', 'd', 'a', 'e'};
    quickSort(arr4.data(), arr4.size());
    assert(arr3 == arr4);

    for (int i = 0; i < TEST_SIZE; i++) {
        arr5[i] = arr6[i] = i;
    }
    for (int i = 0; i < TEST_SIZE; i++) {
        int tmp = arr6[i], idx = rand() % (TEST_SIZE - i) + i;

        arr6[i] = arr6[idx];
        arr6[idx] = tmp;
    }
    
    quickSort(arr6.data(), arr6.size());
    assert(arr5 == arr6);

    cout << "All tests passed!" << endl;
    return 0;
}