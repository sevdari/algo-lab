/*
This only solves the first two test cases but 
serves as a starting point for a full solution.
The idea is similar but with a 6D DP.
It would be a bit annoying to implement it all,
so I leave it as is.
*/

#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <climits>
#include <cassert>

using namespace std;

int n, k, m;
int NONE;
int north_last = -1, south_last = -1;
vector<int> q;
vector<
  vector<
    vector<
      vector<int>>>> memo;

long dp(int i, int diff, int north_last, int south_last){
  
  assert(diff >= - 12 && diff <= 12);
  
  int soldier = q[i];
  if(memo[i][diff+12][north_last][south_last]!=-1)
    return memo[i][diff+12][north_last][south_last];
  
  long north_score = 1000, south_score = 1000;
  
  // north score
  if(north_last != NONE && north_last != soldier)
    north_score += 1000;
  north_score -= pow(2, abs(diff+1));
  
  // south score
  if(south_last != NONE && south_last != soldier)
    south_score += 1000;
  south_score -= pow(2, abs(diff - 1));
  
  if(max(north_score, south_score) < 0){
    memo[i][diff+12][north_last][south_last] = INT_MIN;
    return INT_MIN;
  }
  
  // base case
  if(i == n - 1){
    memo[i][diff+12][north_last][south_last] = max(north_score, south_score);
    return memo[i][diff+12][north_last][south_last];
  }
  
  // recursion
  north_score += dp(i+1, diff+1, soldier, south_last);
  south_score += dp(i+1, diff-1, north_last, soldier);
  memo[i][diff+12][north_last][south_last] = max(north_score, south_score);
  return memo[i][diff+12][north_last][south_last];
}

void testcase() {
  // reset everything
  north_last = -1; south_last = -1;
  
  // read input
  cin>>n>>k>>m;
  q.assign(n, 0);
  for(int i = 0; i < n; i++)
    cin>>q[i];
  NONE = k;
  
  memo = vector<vector<vector<vector<int>>>>(
      n,
      vector<vector<vector<int>>>(
          25,
          vector<vector<int>>(
              k + 1,
              vector<int>(k + 1, -1)
          )
      )
  );

  cout << dp(0, 0, NONE, NONE) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);

  int t;
  cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
