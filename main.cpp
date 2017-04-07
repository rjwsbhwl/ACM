#include<iostream>
#include<vector>
#include<map>
#include<iomanip>
#include<string>
using namespace std;

const double eps = 1e-3;
const int maxn = 8 + 5;
const int maxm = 100 + 5;
const int maxstate = 15000;  // There are at most 8 stocks and each stock can hold at most 8. Use combination to solve the total possible states.
// 8 maybe not used up, we add a breaker. So there are at most C(8 + 8 + 1 - 1, 8) = C(16, 8) = 12870 different states. So 15000 should be safe.

vector<vector<int>> states; // Different states.
map<vector<int>, int> ID;  // Each ID represents one state.
double d[maxm][maxstate], c, price[maxn][maxm];  // d[i][s] represents the max cash can have at the i-th day(0 base index) with stock combination s.
int n, m, k[maxn], kk, buy_next[maxstate][maxn], sell_next[maxstate][maxn], opt[maxm][maxstate], pre[maxm][maxstate];
// buy_next/sell_next[i][s] represents the next state if buy/sell one s at state i.
// opt[day][state] represents the action need to make to transfer to [day][state]. pre denotes the previous state.
string name[maxn];

void dfs(int cur, vector<int>& lot, int totlot) {  // cur: the current stock being considered; totlot: the total lot holding now
    if(cur == n) {  // If have considered all stocks, we get a new state.
        ID[lot] = states.size();
        states.push_back(lot);
        return;
    }

    for(int i = 0; i <= k[cur] && totlot + i <= kk; i++) {  // Consider each stock's possible number of lots.
        lot[cur] = i;
        dfs(cur + 1, lot, totlot + i);
    }
}

void init() {
    vector<int> lot(n);  // record each stock and its number being held.
    states.clear();
    ID.clear();
    dfs(0, lot, 0); // The function to get each state.
    for(int i = 0; i < states.size(); i++) {
        int totlot = 0;
        for(int j = 0; j < n; j++) totlot += states[i][j];  // Get the current total number of lots.
        for(int j = 0; j < n; j++) {
            buy_next[i][j] = -1; sell_next[i][j] = -1;  // Initialize the two array. We can judge whether we can do this action(buy or sell) based on whether it's -1 or not later.
            // Buy:
            if(states[i][j] < k[j] && totlot < kk) {
                vector<int> new_state = states[i];
                new_state[j]++;
                buy_next[i][j] = ID[new_state];
            }
            // Sell:
            if(states[i][j]) {
                vector<int> new_state = states[i];
                new_state[j]--;
                sell_next[i][j] = ID[new_state];
            }
        }
    }
}

void update(int day, int cur, int tom, double v, int op) {  // op: negative: sell; 0: hold; positive: buy
    if(v > d[day+1][tom]) {  // If the value is larger, then change.
        d[day+1][tom] = v;
        pre[day+1][tom] = cur;  // Record the previous state.
        opt[day+1][tom] = op;  // Record the option.
    }
}

double dp() {
    for(int day = 0; day <= m; day++)
        for(int s = 0; s < states.size(); s++) d[day][s] = -1;  // Initialize to a negative number.

    d[0][0] = c;  // The 0-th day, without stock, with money c.
    for(int day = 0; day < m; day++)
        for(int s = 0; s < states.size(); s++) {
            double v = d[day][s];
            if(v < -0.5) continue;  // Cannot get to this state

            update(day, s, s, v, 0);  // Hold
            for(int i = 0; i < n; i++) {
                if(buy_next[s][i] >= 0 && v >= price[i][day] - eps)  // Since v can be smaller after calculation, we use an eps
                    update(day, s, buy_next[s][i], v - price[i][day], i + 1);  // Buy
                if(sell_next[s][i] >= 0)
                    update(day, s, sell_next[s][i], v + price[i][day], -i - 1);  // Sell
            }
        }
    return d[m][0];  // Finally we don't have stocks
}

void print(int day, int s) {
    if(!day) return;  // No info. when day == 0 since pre and opt are recorded one day after cur
    print(day-1, pre[day][s]);  // The previous day and state
    if(!opt[day][s]) cout << "HOLD" << endl;
    else if(opt[day][s] > 0) cout << "BUY" << ' ' << name[opt[day][s]-1] << endl;
    else cout << "SELL" << ' ' << name[-opt[day][s]-1] << endl;
}

int main() {
    int kase = 0;
    while(cin >> c >> m >> n >> kk) {
        if(kase++) cout << endl;
        int size;
        for(int i = 0; i < n; i++) {
            cin >> name[i] >> size >> k[i];
            for(int j = 0; j < m; j++) { cin >> price[i][j]; price[i][j] *= size; }  // Get the total price for one lot.
        }
        init();

        cout << fixed;
        cout << setprecision(2) << dp() << endl;
        print(m, 0);
    }
    return 0;
}