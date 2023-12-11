#include <iostream>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    int a[N][M] = { 0 };

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) { 
            cin >> a[i][j]; 
        }
    }

    for (int i = N - 2; i >= 0; i--) { a[i][M - 1] += a[i + 1][M - 1]; }
    for (int j = M - 2; j >= 0; j--) { a[N - 1][j] += a[N - 1][j + 1]; }

    for (int i = N - 2; i >= 0; i--) {
        for (int j = M - 2; j >= 0; j--) { 
            a[i][j] += min(a[i + 1][j], a[i][j + 1]);
        }
    }

    cout << a[0][0] << endl;

    return 0;
}