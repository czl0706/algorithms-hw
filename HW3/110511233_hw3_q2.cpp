#include <iostream>
using namespace std;

inline int min(int a, int b, int c) {
    return min(min(a, b), c);
}

int main() {
    string s1, s2;
    cin >> s1 >> s2;
    int s1_len, s2_len;

    s1_len = s1.length();
    s2_len = s2.length();

    int dp[s1_len + 1][s2_len + 1] = { 0 };

    for (int i = 0; i <= s1_len; i++) { dp[i][0] = i; }
    for (int j = 0; j <= s2_len; j++) { dp[0][j] = j; }

    for (int i = 1; i <= s1_len; i++) {
        for (int j = 1; j <= s2_len; j++) {
            dp[i][j] = s1[i-1] == s2[j-1] ? dp[i-1][j-1]: 
                                        1 + min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]);
        }
    }

    cout << dp[s1_len][s2_len] << endl;

    return 0;
}