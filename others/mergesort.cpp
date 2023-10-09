#include <iostream>
using namespace std;

template <typename T>
void merge(T *arr, T* tmpArr, int left, int mid, int right) {
    int leftLen = mid - left + 1, rightLen = right - mid;

    for (int i = 0; i < right - left + 1; i++) {
        tmpArr[i] = arr[left + i];
    }
    
    int i = 0, j = 0, k = left;
    T *leftArr = tmpArr;
    T *rightArr = (tmpArr + leftLen);

    while (i < leftLen && j < rightLen) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < leftLen) {
        arr[k++] = leftArr[i++];
    }
    while (j < rightLen) {
        arr[k++] = rightArr[j++];
    }
}

template <typename T>
void _mergesort(T *arr, T* tmpArr, int left, int right) {
    if (left >= right) {
        return; 
    }
    
    int mid = (left + right) / 2;
    _mergesort(arr, tmpArr, left, mid); 
    _mergesort(arr, tmpArr, mid + 1, right);
    merge(arr, tmpArr, left, mid, right);
}

template <typename T>
void mergeSort(T *arr, int size) {
    T *tmpArr = new T[size];
    _mergesort(arr, tmpArr, 0, size - 1);
    delete [] tmpArr;
}

#include <assert.h>
#include <array>

#define TEST_SIZE 5000000
std::array<int, TEST_SIZE> arr5, arr6;

int main() {
    srand(time(NULL));

    std::array<int, 5> arr1 {1, 2, 3, 4, 5}, arr2 {4, 5, 3, 2, 1};
    mergeSort(arr2.data(), arr2.size());
    assert(arr1 == arr2);

    std::array<char, 5> arr3 {'a', 'b', 'c', 'd', 'e'}, arr4 {'b', 'c', 'd', 'a', 'e'};
    mergeSort(arr4.data(), arr4.size());
    assert(arr3 == arr4);

    for (int i = 0; i < TEST_SIZE; i++) {
        arr5[i] = arr6[i] = i;
    }
    for (int i = 0; i < TEST_SIZE; i++) {
        int tmp = arr6[i], idx = rand() % (TEST_SIZE - i) + i;

        arr6[i] = arr6[idx];
        arr6[idx] = tmp;
    }
    
    mergeSort(arr6.data(), arr6.size());
    assert(arr5 == arr6);

    cout << "All tests passed!" << endl;
    return 0;
}