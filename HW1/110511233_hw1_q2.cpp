#include <iostream>
#include <string>
using namespace std;

template <typename T>
void sink(T *heapArr, int idx, int size) {
    int left, right, smaller;
    while ((left = idx * 2 + 1) < size) {
        right = left + 1;
        if (right < size && heapArr[right] < heapArr[left]) {
            smaller = right;
        } else {
            smaller = left;
        }

        if (heapArr[idx] > heapArr[smaller]) {
            T tmp = heapArr[idx];
            heapArr[idx] = heapArr[smaller];
            heapArr[smaller] = tmp;
            idx = smaller;
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
void topK_heapSort(T *arr, int size, int k) {
    heapify(arr, size);

    for (int i = size - 1; i >= (size - k); i--) {
        T tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;
        sink(arr, 0, i);
    }
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    int N, k;
    cin >> N >> k;

    int arr[N], total = 0;
    for (int i = 0, x, y; i < N; i++) {
        cin >> x >> y;
        arr[i] = x - y;
        total += y;
    }

    topK_heapSort(arr, N, k);

    // top K smallest values are stored in reverse order
    for (int i = (N - k); i < N; i++) { total += arr[i]; }
    
    cout << total << endl;
    return 0;
}