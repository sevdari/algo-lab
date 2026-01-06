#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

typedef pair<int, int> PII;

vector<vector<PII>> canals;
vector<vector<long>> memo;
long n, edges, score_to_beat, moves; 

long dp(int m, int hole){
  if(m == 0) return 0;
  if(memo[m][hole]!=-1) return memo[m][hole];
  
  if(canals[hole].empty()){
    return dp(m, 0);
  }
  
  for(auto canal: canals[hole]){
    int neigh = canal.first, score = canal.second;
    long transition = memo[m-1][neigh] != -1 ? memo[m-1][neigh] : dp(m-1, neigh);
    if(transition==-1) continue;
    memo[m][hole] = max(memo[m][hole], score + transition);
  }
  return memo[m][hole];
}

void testcase() {
  // read input
  cin>>n>>edges>>score_to_beat>>moves;
  canals.assign(n, vector<PII>());
  for(int i = 0; i < edges; i++){
    int u, v, p; cin>>u>>v>>p;
    canals[u].push_back({v, p});
  }
  // start solution
  memo.assign(moves + 1, vector<long>(n, -1));
  long res = 0, m = 1;
  while(m <= moves){
    res = dp(m, 0);
    if(res >= score_to_beat){
      cout << m << endl;
      return;
    }
    m += 1;
  }
  cout << "Impossible\n";
  
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
