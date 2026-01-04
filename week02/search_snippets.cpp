#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

typedef pair<int, int> PII;

void testcase() {
  int n; cin>>n;
  vector<int> m(n, 0);
  for(int i = 0; i < n; i++)
    cin >> m[i];
  
  vector<PII> pos;
  
  int temp;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m[i]; j++){
      cin>>temp;
      pos.push_back({temp, i});
    }
  }
  
  sort(pos.begin(), pos.end());
  
  vector<int> freq(n, 0);
  int count = 0;
  
  int left = 0, right = 0, best = pos.back().first, id;
  
  while(right < pos.size()){
    
    id = pos[right].second;
    if(freq[id] == 0) count++;
    freq[id]++;
    
    if(count == n){
      best = min(best, pos[right].first - pos[left].first + 1);
      while(left < right){
        id = pos[left].second;
        left++; freq[id]--;
        if(freq[id] == 0){
          count--; break;
        } else {
          best = min(best, pos[right].first - pos[left].first + 1);
        }
      }
    }
    right++;
  }
  
  cout << best << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
