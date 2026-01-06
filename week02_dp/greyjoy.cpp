///1
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

long n, k, w; 

long window_scan(const vector<long>& waterway){
  long left = 0, right = 0, current = 0, best = 0;
  while(right < waterway.size()){
    current += waterway[right];
    if(current == k){
      best = max(best, right - left + 1);
      current -= waterway[left]; 
      left++; right++;
    } else if (current < k){
      right ++;
    } else {
      while(current > k){
        current -= waterway[left];
        left ++;
      }
      current -= waterway[right];
    }
  }
  return best;
}

void testcase() {
  cin>>n>>k>>w;
  
  vector<long> islands(n, 0);
  vector<vector<long>> waterways(w, vector<long>());
  for(long i = 0; i < n; i++)
    cin>>islands[i];
  
  for(long i = 0; i < w; i++){
    long l, temp; cin>>l;
    for(long j = 0; j < l; j++){
      cin>>temp; waterways[i].push_back(islands[temp]);
    }
  }
  
  long best = 0;
  
  // window scans
  for(long i = 0; i < w; i++)
    best = max(best, window_scan(waterways[i]));
    
  // combining waterways
  vector<long> memo(k, 0);
  for(long i = 0; i < w; i++){
    long current = 0, j = 0;
    
    // check for valid combinations
    while(j < waterways[i].size() && current < k){
      current += waterways[i][j];
      if(k - current > 0 && memo[k-current] != 0)
        best = max(best, j + 1 + memo[k-current]);
      j++;
    }
    
    // update history
    current = 0; j = 1;
    while(j < waterways[i].size() && current < k){
      current += waterways[i][j];
      if(current < k)
        memo[current] = max(memo[current], j);
      j++;
    }
    
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
