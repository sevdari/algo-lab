#include<bits/stdc++.h>

using namespace std;

void testcase(){
  int n, m; cin>>n>>m;
  vector<vector<long>> board(n, vector<long>(m, 0));
  
  // read input
  for(int i = 0; i < n; i++){
    long current_sum = 0;
    for(int j = 0; j < m; j++){
      cin>>board[i][j];
      long original = board[i][j];
      board[i][j] += current_sum; current_sum += original;
      if(i > 0) board[i][j] += board[i-1][j];
    }
  }
  
  vector<vector<long>> cost(n, vector<long>(m, LONG_MAX));
  cost[0][0] = 0;
  
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      if(i == 0 && j == 0) continue;
      if(i == 0){
        cost[i][j] = cost[i][j-1] + board[n-1][j-1] - board[0][j-1];
      } else if (j == 0){
        cost[i][j] = cost[i-1][j] + board[i-1][m-1] - board[i-1][0];
      } else {
        // cost from down move
        long down_cost = cost[i-1][j] + board[i-1][m-1] - board[i-1][j];
        // cost from right move
        long right_cost = cost[i][j-1] + board[n-1][j-1] - board[i][j-1];
        cost[i][j] = min(down_cost, right_cost);
      }
    }
  }
  
  cout << cost[n-1][m-1] << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}