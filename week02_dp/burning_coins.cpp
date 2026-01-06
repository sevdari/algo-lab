#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

vector<vector<int>> memo(2500, vector<int>(2500));
vector<int> coins(2500);
int n;

int dp(int start, int end){
  // base cases
  if(start > end || start >= n || end < 0) return 0;
  if(memo[start][end] != -1) return memo[start][end];
  if(start == end-1) return max(coins[start], coins[end]);
  if(start == end) return coins[start];
  
  // logic
  int take_first = coins[start] + min(dp(start + 2, end), dp(start + 1, end - 1));
  int take_last = coins[end] + min(dp(start, end - 2), dp(start + 1, end - 1));
  
  // update memo and return
  int best = max(take_first, take_last);
  memo[start][end] = best;
  return best;
}

void testcase(){
  // read input
  cin>>n;
  for(int i = 0; i < n; i++)
    cin>>coins[i];
  // reset memo
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      memo[i][j] = -1;
    }
  }
  // run dp
  cout<<dp(0, n-1)<<endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  while(t){testcase(); t--;}
}