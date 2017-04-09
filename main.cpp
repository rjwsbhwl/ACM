#include<iostream>
#include<string>
using namespace std;

// 使用刷表法解决。d[i][x][y]表示把前i位排好，第i+1、i+2位的数字为x、y时所需要的最小花费。因为把第i位排好的时候所能带给后面数的影响只有第i+1、i+2位，所以用此状态来记录。
const int maxn = 1000 + 10;
const int maxd = 10 + 5;
const int INF = 100000;  // INF need to be big enough. Since there are at most 1000 chars and each need at most 9 manipulation, 10000 should already be ok.
int d[maxn][maxd][maxd], origin[maxn], aim[maxn];
string s1, s2;

int main() {
    while(cin >> s1 >> s2) {
        int n = s1.length();
        for(int i = 0; i < n; i++) {
            origin[i+1] = s1[i] - '0';
            aim[i+1] = s2[i] - '0';
        }
        origin[n+1] = origin[n+2] = aim[n+1] = aim[n+2] = 0;  // Control the range of the n+1 and n+2 position.
        for(int i = 0; i <= n; i++)
            for(int j = 0; j < 10; j++)
                for(int k = 0; k < 10; k++) d[i][j][k] = INF;  // Initialization
        d[0][origin[1]][origin[2]] = 0;  // The first state, the only thing we have.

        for(int i = 0; i < n; i++)
            for(int j = 0; j < 10; j++)
                for(int k = 0; k < 10; k++) {
                    int p = (aim[i+1] - j + 10) % 10;  // Steps need to do in order to get aim[i+1].
                    for(int x = 0; x <= p; x++)  // The i+2 can do at most p steps.
                        for(int y = 0; y <= x; y++)  // The i+3 can do at most x steps.
                            d[i+1][(k+x)%10][(origin[i+3]+y)%10] = min(d[i+1][(k+x)%10][(origin[i+3]+y)%10], d[i][j][k] + p);  // Update.

                    p = 10 - p;  // Opposite direction
                    for(int x = 0; x <= p; x++)
                        for(int y = 0; y <= x; y++)
                            d[i+1][(k-x+10)%10][(origin[i+3]-y+10)%10] = min(d[i+1][(k-x+10)%10][(origin[i+3]-y+10)%10], d[i][j][k] + p);  // Update
                }
        cout << d[n][0][0] << endl;  // The final ans.
    }
    return 0;
}