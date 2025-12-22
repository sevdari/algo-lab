#include<iostream>
#include<algorithm>
#include<vector>
#include <climits>
#include <numeric>

using namespace std;

vector<vector<long>> memo(1000, vector<long>(1000, -1));
vector<int> a(1000), b(1000);



long dp(int a_top, int b_top){
  // base cases
  if(memo[a_top][b_top]!=-1) return memo[a_top][b_top];
  if(a_top==1 || b_top==1){
    memo[a_top][b_top] = (a[a_top-1] - a_top) * (b[b_top-1] - b_top);
    return memo[a_top][b_top];
  }
  // dp logic
  long min_cost = LONG_MAX;
  for(int i = 1; i < a_top; i++){
    for(int j = 1; j < b_top; j++){
      long sum_a = a[a_top-1] - a[i-1];
      long sum_b = b[b_top-1] - b[j-1];
      long temp_cost = (sum_a - (a_top - i)) * (sum_b - (b_top - j));
      temp_cost += memo[i][j] != -1 ? memo[i][j] : dp(i, j);
      if(temp_cost < min_cost) min_cost = temp_cost;
    }
  }
  memo[a_top][b_top] = min_cost;
  return memo[a_top][b_top];
}

void testcase(){
  int n; cin>>n;
  for(int i = 0; i <= n; i++){
    for(int j = 0; j <= n; j++){
      memo[i][j] = -1;
    }
  }
  for(int i = 0; i < n; i++)
    cin>>a[i];
  for(int i = 1; i < n; i++)
    a[i] += a[i-1];
  for(int i = 0; i < n; i++)
    cin>>b[i];
  for(int i = 1; i < n; i++)
    b[i] += b[i-1];
  
  cout << dp(n, n) << endl;
}


int main(){
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  while(t){
    testcase();
    t--;
  }
}