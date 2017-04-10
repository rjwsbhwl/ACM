// Must get to the treasure before the deadline!
// The start time is 0.
#include<iostream>
using namespace std;

const int maxn = 10000 + 10;
const int INF = int(1e8);


// Without sliding array:
// d[i][j][0] means the minimal time to collect all treasures in the interval [i, j] and ends at i.
// d[i][j][1] means ends at right.
// a[i]: the location of the i-th object; b[j]: The deadline of the i-th object.
// The formulas of the two value functions are:
// d[i][j][0] = min(d[i+1][j][0] + a[i+1] - a[i], d[i+1][j][1] + a[j] - a[i]);
// d[i][j][1] = min(d[i][j-1][0] + a[j] - a[i], d[i][j-1][1] + a[j] - a[j-1]);
// If cannot arrive at the treasure before the deadline, then simply set it to INF.

/*int d[maxn][maxn][2], a[maxn], b[maxn], n;

int main() {
    while(cin >> n) {
        for(int i = 0; i < n; i++) {
            cin >> a[i] >> b[i];
            d[i][i][0] = d[i][i][1] = (b[i] > 0) ? 0 : INF;
        }
        for(int i = n-2; i >= 0; i--)
            for(int j = i + 1; j < n; j++) {
                d[i][j][0] = min(d[i+1][j][0] + a[i+1] - a[i], d[i+1][j][1] + a[j] - a[i]);
                if(d[i][j][0] >= b[i]) d[i][j][0] = INF;
                d[i][j][1] = min(d[i][j-1][0] + a[j] - a[i], d[i][j-1][1] + a[j] - a[j-1]);
                if(d[i][j][1] >= b[j]) d[i][j][1] = INF;
            }
        int ans = min(d[0][n-1][0], d[0][n-1][1]);
        if(ans >= INF) cout << "No solution" << endl;
        else cout << ans << endl;
    }
    return 0;
}*/

// With sliding array:
// Every time we calculate d[i][j][0/1], we only need the interval whose size is 1 unit smaller than it. It means we only have to save 1 value and we just keep d[][] to save memory.
int d[maxn][2], a[maxn], b[maxn], n;

int main() {
    while(cin >> n) {
        for(int i = 0; i < n; i++) {
            cin >> a[i] >> b[i];
            d[i][0] = d[i][1] = (b[i] > 0) ? 0 : INF;
        }

        for(int i = n-2; i >= 0; i--)
            for(int j = i+1; j < n; j++) {
                d[j][0] = min(d[j][0] + a[i+1] - a[i], d[j][1] + a[j] - a[i]);
                if(d[j][0] >= b[i]) d[j][0] = INF;
                d[j][1] = min(d[j-1][0] + a[j] - a[i], d[j-1][1] + a[j] - a[j-1]);
                if(d[j][1] >= b[j]) d[j][1] = INF;
            }

        int ans = min(d[n-1][0], d[n-1][1]);
        if(ans >= INF) cout << "No solution" << endl;
        else cout << ans << endl;
    }
    return 0;
}
