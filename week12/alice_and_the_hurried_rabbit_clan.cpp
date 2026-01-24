///1
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

void testcase() {
  int n, m; cin>>n>>m;
  vector<vector<long>> rabbits(n, vector<long>(m, 0));
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      cin>>rabbits[i][j];
    }
  }
  vector<vector<long>> prefix_sum(n, vector<long>(m, 0));
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      prefix_sum[i][j] += rabbits[i][j];
      if(i == 0 && j == 0) continue;
      if(j == 0)
        prefix_sum[i][j] += prefix_sum[i-1][j];
      else if(i == 0)
        prefix_sum[i][j] += prefix_sum[i][j-1];
      else
        prefix_sum[i][j] += prefix_sum[i][j-1] + prefix_sum[i-1][j] - prefix_sum[i-1][j-1];
    }
  }
  
  vector<vector<long>> min_cost(n, vector<long>(m, 0));
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      if(i == 0 && j == 0) continue;
      if(i == 0)
        min_cost[i][j] += min_cost[i][j-1] + prefix_sum[n-1][j-1] - prefix_sum[0][j-1];
      else if(j == 0)
        min_cost[i][j] += min_cost[i-1][j] + prefix_sum[i-1][m-1] - prefix_sum[i-1][j];
      else{
        long right = min_cost[i][j-1] + prefix_sum[n-1][j-1] - prefix_sum[i][j-1];
        long down = min_cost[i-1][j] + prefix_sum[i-1][m-1] - prefix_sum[i-1][j];
        min_cost[i][j] = min(right, down);
      }
    }
  }
  
  cout << min_cost[n-1][m-1] << endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
