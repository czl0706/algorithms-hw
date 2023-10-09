#include <iostream>
using namespace std;

template <typename T>
void sink(T *heapArr, int idx, int size) {
    int left, right, larger;
    while ((left = idx * 2 + 1) < size) {
        right = left + 1;
        if (right < size && heapArr[right] > heapArr[left]) {
            larger = right;
        } else {
            larger = left;
        }

        if (heapArr[idx] < heapArr[larger]) {
            T tmp = heapArr[idx];
            heapArr[idx] = heapArr[larger];
            heapArr[larger] = tmp;
            idx = larger;
        } else {
            break;
        }
    }
}

template <typename T>
void heapify(T *arr, int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        sink(arr, i, size);
    }
}

template <typename T>
void heapSort(T *arr, int size) {
    heapify(arr, size);

    for (int i = size - 1; i >= 1; i--) {
        T tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;
        sink(arr, 0, i);
    }
}

#include <assert.h>
#include <array>

#define TEST_SIZE 5000000
std::array<int, TEST_SIZE> arr5, arr6;

int main() {
    srand(time(NULL));

    std::array<int, 5> arr1 {1, 2, 3, 4, 5}, arr2 {4, 5, 3, 2, 1};
    heapSort(arr2.data(), arr2.size());
    assert(arr1 == arr2);

    std::array<char, 5> arr3 {'a', 'b', 'c', 'd', 'e'}, arr4 {'b', 'c', 'd', 'a', 'e'};
    heapSort(arr4.data(), arr4.size());
    assert(arr3 == arr4);

    for (int i = 0; i < TEST_SIZE; i++) {
        arr5[i] = arr6[i] = i;
    }
    for (int i = 0; i < TEST_SIZE; i++) {
        int tmp = arr6[i], idx = rand() % (TEST_SIZE - i) + i;

        arr6[i] = arr6[idx];
        arr6[idx] = tmp;
    }
    
    heapSort(arr6.data(), arr6.size());
    assert(arr5 == arr6);

    cout << "All tests passed!" << endl;
    return 0;
}