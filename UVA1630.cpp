#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
using namespace std;

const int maxn = 100 + 10;
const int INF = int(1e4);

string str, fold[maxn][maxn];  // fold[l][r] represents the string in [l, r] after compressing
int d[maxn][maxn];  // d[l][r] represents the minimum length of string in [l, r] after compressing

int judge(int l, int r) {
    int len = r - l + 1;  //The length of the string
    for(int i = 1; i <= len; i++) {  // i is possible period
        if(len % i) continue;  // A period must be a factor of the length
        bool ok = true;  // A flag
        for(int j = l; j+i <= r; j++)
            if(str[j] != str[j+i]) {  // i is not a period
                ok = false;
                break;
            }
        if(ok) return i;  // i is a period
    }
    return 0;  // is isn't a period
}

// In total, there are three cases: 1. There is only one character remaining, then d = 1 and fold = str[current]
// 2. The string is constructed by putting two strings together
// 3. The string itself has period != 1, which can be compressed

int dp(int l, int r) {
    int& ans = d[l][r];
    if(ans != -1) return ans;  // dp procedure

    // Case 1:
    if(l == r) {
        fold[l][r] = str[l];
        return ans = 1;
    }

    // Case 2:
    ans = INF;  // Since we have to get the minimum value, initialize ans as INF
    for(int i = l; i < r; i++) {
        int tmp = dp(l, i) + dp(i+1, r);
        if(tmp < ans) { ans = tmp; fold[l][r] = fold[l][i] + fold[i+1][r]; }  // Update
    }

    // Case 3:
    int cycle = judge(l, r);  // Judge function is to judge the period of the current string
    if(!cycle) return ans;  // No period
    char buffer[10];  // Get the coefficient
    sprintf(buffer, "%d", (r-l+1)/cycle);
    string num(buffer);
    string now = num + '(' + fold[l][l+cycle-1] + ')';  // Constructed by three parts
    if(now.length() < ans) {  // If we get a better result, then update d and fold
        ans = now.length();
        fold[l][r] = now;
    }
    // The reason why we won't get something like 2(2(ABCD)) from "ABCDABCDABCDABCD" is that every time we use f[l][l+cycle-1],
    // which starts from the first character and spans for 1 period. It won't have brackets
    return ans;
}

int main() {
    while(cin >> str) {
        memset(d, -1, sizeof(d));  // Reset the array
        dp(0, str.length()-1);  // Use recurrence to solve the minimum length
        //cout << fold[0][7] << endl;
        cout << fold[0][str.length()-1] << endl;  // Output the string after compressing
    }
    return 0;
}
