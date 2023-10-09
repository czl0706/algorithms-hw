#include <iostream>
#include <string>
using namespace std;

bool cmp(const string &a, const string &b) {
    int minLen = min(a.length(), b.length());
    
    for (int i = 0; i < minLen; i++) {
        if (a[i] != b[i]) { return a[i] > b[i]; }
    }
    
    return (a + b) > (b + a);
}

template <typename T>
int _partition(T *arr, int left, int right) {
    T tmp, x = arr[right]; // arr[right] is the pivot
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (cmp(arr[j], x)) {
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
void _quicksort(T *arr, int left, int right) {
    if (left >= right) {
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

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    int N;
    cin >> N;

    string arr[N];
    for (int i = 0; i < N; i++) { cin >> arr[i]; }

    quickSort(arr, N);

    for (int i = 0; i < N; i++) { cout << arr[i]; }
    
    return 0;
}