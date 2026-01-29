#include<bits/stdc++.h>

using namespace std;

int n, m, k;

void testcase(){
  cin>>n>>m>>k;
  vector<int> v(n);
  for(int i = 0; i < n; i++)
    cin>>v[i];
  
  // precompute valid windows
  vector<pair<int, int>> windows;
  int right = 0, left = 0, sum = v[0];
  while(right < n){
    if(sum==k){
      windows.push_back({left, right - left + 1});
      sum -= v[left]; left++;
      if(left > right){
        if(left == n) break;
        right = left;
        sum = v[left];
      }
    } else if (sum < k){
      right += 1;
      if(right == n) break;
      sum += v[right];
    } else {
      sum -= v[left];
      left++;
      if(left > right){
        if(left == n) break;
        right = left;
        sum = v[left];
      }
      
    }
  }
  
  int w = windows.size();
  if(w < m) { cout << "fail\n"; return; }
  vector<int> max_from(w);
  
  for(int i = w - 1; i >= 0; i--){
    auto [start, length] = windows[i];
    if(i == w - 1) max_from[i] = length;
    else max_from[i] = max(length, max_from[i+1]);
  }
  
  for(int iter = 1; iter < m; iter++){
    vector<int> next_max_from(w, -1);
    for(int i = 0; i < w; i++){
      auto [start, length] = windows[i];
      auto next = lower_bound(windows.begin(), windows.end(), make_pair(start+length, 0));
      if(next==windows.end()) continue;
      int temp = max_from[next - windows.begin()];
      if(temp!=-1)
        next_max_from[i] = length + temp;
    }
    max_from = next_max_from;
    for(int i = w - 2; i >= 0; i--)
      max_from[i] = max(max_from[i], max_from[i+1]);
  }
  
  int best = *max_element(max_from.begin(), max_from.end());
  if(best == -1)
    cout << "fail\n";
  else
    cout << best << endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}