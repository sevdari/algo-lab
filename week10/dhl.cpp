#include<bits/stdc++.h>

using namespace std;

void testcase(){
  int n; cin>>n;
  vector<int> a(n + 1), b(n + 1);
  a[0] = 0; b[0] = 0;
  
  for(int i = 1; i < n + 1; i++){
    cin>>a[i];
    a[i] += a[i-1];
  }
  for(int i = 1; i < n + 1; i++){
    cin>>b[i];
    b[i] += b[i-1];
  }
  
  // cost of pick up having i elements from a and j elements from b
  vector<vector<int>> memo(n + 1, vector<int>(n+1, INT_MAX));
  memo[0][0] = 0; // base case
  
  
  for(int i = 1; i < n + 1; i++){
    for(int j = 1; j < n + 1; j++){
      if(i == 1)
        memo[i][j] = (a[1] - 1) * (b[j] - j);
      else if(j == 1)
        memo[i][j] = (a[i] - i) * (b[1] - 1);
      else{
        // take all
        memo[i][j] = (a[i] - i) * (b[j] - j); 
        // take the top element from a
        for(int k = 1; k < j; k++){
          memo[i][j] = min(
            memo[i][j], 
            memo[i-1][k] + (a[i] - a[i-1] - 1) * (b[j] - b[k] - (j - k))
          );
        }
        // take the top element from b
        for(int k = 1; k < i; k++){
          memo[i][j] = min(
            memo[i][j], 
            memo[k][j-1] + (a[i] - a[k] - (i - k)) * (b[j] - b[j-1] - 1)
          );
        }
      }
    }
  }
  
  cout << memo[n][n] << endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;

}

