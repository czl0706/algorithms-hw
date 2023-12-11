#include<iostream>
using namespace std;

const int MOD = 1000000007;

int main() {
    int N;
    cin >> N;
    long long int dp[N + 1][2] = { 0 };

	dp[0][0] = 1;
	dp[1][0] = 1;

	for (int i = 2; i <= N; i++) {
		dp[i][0] = (dp[i - 1][0] + dp[i - 1][1] * 2 + dp[i - 2][0]) % MOD;
		dp[i][1] = (dp[i - 2][0] + dp[i - 1][1]) % MOD;
	}

    cout << dp[N][0] << endl;
    return 0;
}