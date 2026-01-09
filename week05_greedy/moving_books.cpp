#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

void testcase() {
  // read input
  int n, m; cin>>n>>m;
  vector<int> friends(n, 0), boxes(m, 0);
  
  for(int i = 0; i < n; i++)
    cin>>friends[i];
  for(int i = 0; i < m; i++)
    cin>>boxes[i];
  
  // solution
  sort(friends.begin(), friends.end(), greater<int>());
  sort(boxes.begin(), boxes.end(), greater<int>());
  
  if(friends[0] < boxes[0]){
    cout << "impossible\n"; return;
  }
  
  vector<int> val(m, friends[0]), ind(m, 0);
  int best = 0;
  
  for(auto box: boxes){
    int i = lower_bound(val.begin(), val.end(), box) - val.begin();
    best = max(best, i + 1);
    ind[i]++;
    if(ind[i] >= n)
      val[i] = -1;
    else
      val[i] = friends[ind[i]];
  }

  cout << best * 3 - 1 << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
