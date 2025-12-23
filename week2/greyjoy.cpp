#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

const int maxn = 5e4 + 5, maxval = 1e6 + 5;

typedef long long ll;

int c[maxn];
int dp[maxval];
int a[maxn];

void solve() {
  int n, k, w;
  cin >> n >> k >> w;
  for(int i = 0; i < n; i++) {
    cin >> c[i];
  }
  int sol = 0;
  ll sum;
  int l;
  for(int i = 0; i < w; i++) {
    cin >> l;
    sum = 0;
    for(int j = 0; j < l; j++) {
      cin >> a[j];
      sum += c[a[j]];
      if(sum <= k && dp[k - sum] != -1) {
        sol = max(sol, dp[k - sum] + j + 1);
      }
    }
    sum = 0;
    for(int j = 1; j < l; j++) {
      sum += c[a[j]];
      if(sum < k) {
        dp[sum] = max(dp[sum], j);
      }
    }
    int x = 0;
    sum = 0;
    for(int j = 0; j < l; j++) {
      sum += c[a[j]];
      while(sum > k) {
        sum -= c[a[x]];
        x++;
      }
      if(sum == k) {
        sol = max(sol, j - x + 1);
      }
    }
  }
  
  cout << sol << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  int t;
  cin >> t;
  while(t--) {
    memset(dp, -1, sizeof(dp));
    dp[0] = 0;
    solve();
  }
  return 0;
}
