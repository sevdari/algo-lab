///1
#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

void testcase(){
  long n; cin>>n;
  vector<long> m(n, 0);
  vector<pair<long, long>> positions;
  for(long i = 0; i < n; i++){
    cin>>m[i];
  }
  for(long i = 0; i < n; i++){
    for(long j = 0; j < m[i]; j++){
      long pos; cin>>pos;
      positions.push_back({pos, i});
    }
  }
  sort(positions.begin(), positions.end());
  
  long count = 0, best = positions.back().first;
  vector<long> freq(n, 0);
  long left = 0, right = 0;
  while(right < positions.size() || count == n){
    if(count < n){
      auto [pos, word] = positions[right];
      if(freq[word] == 0) count++;
      freq[word] += 1;
      right += 1;
    } else {
      auto [pos, word] = positions[left];
      best = min(best, positions[right-1].first - pos + 1);
      if(freq[word] == 1) count--;
      freq[word] -= 1;
      left += 1;
      if(left > right) right = left;
    }
  }
  
  cout << best << endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int tests; cin>>tests;
  while(tests--){
    testcase();
  }
}