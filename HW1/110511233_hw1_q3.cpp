#include <iostream>
#include <algorithm>
#include <map>
#include <string>
using namespace std;

struct bag {
    int start, end, exp;
};

bool cmp(bag a, bag b) {
    return a.end < b.end;
}

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
        // if (leftArr[i] <= rightArr[j]) {
        if (cmp(leftArr[i], rightArr[j])) {
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
    if (left >= right) { return; }
    
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

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;

    bag arr[m];
    for (int i = 0; i < m; i++) { cin >> arr[i].start >> arr[i].end >> arr[i].exp; }

    mergeSort(arr, m);

    int maxExp = 0;
    map<int, int> dp{ make_pair(0, 0) };

    for (int i = 0; i < m; i++) {
        // auto it = dp.upper_bound(arr[i].start - 1);
        // int x = (--it)->second + arr[i].exp;

        int x = prev(dp.upper_bound(arr[i].start - 1))->second + arr[i].exp;
        if (x > maxExp) {
            maxExp = x;
            dp[arr[i].end] = maxExp;
        }
    }

    cout << maxExp << endl;
    return 0;
}