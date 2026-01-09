/*
TODO: This is quity messy! Clean up later!
*/
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

long n, m;

void print(const vector<long>& v){
  for(size_t i = 0; i < v.size(); i++)
    cout << v[i] << " ";
  cout << endl;
}

long find_segment_with_fewest_overlaps(
    const vector<long>& starts,
    const vector<long>& finishes
  ){
    size_t s = 0, f = 0, count = 0, current = 1;
    while(s < starts.size() && starts[s] == current){
      count++; s++;
    }
    size_t best_count = count, best_segment = current;
    
    while(f < finishes.size() && finishes[f]!=m+1){
      if(s >= starts.size()){
        count--; current = finishes[f]; f++; 
        if(count < best_count){
          best_count = count; best_segment = current;
        }
        continue;
      }
      if(starts[s] == finishes[f]){
        s++; f++; continue;
      } else if (starts[s] > finishes[f]){
        count--; current = finishes[f]; f++; 
        if(count < best_count){
          best_count = count; best_segment = current;
        }
      } else {
        count++; s++;
      }
    }
    return best_segment;
}

void testcase() {
  cin >> n >> m;
  vector<pair<long, long>> jedis;
  vector<long> starts, finishes;
  
  for(long i = 0; i < n; i++){
    long a, b; cin>>a>>b;
    jedis.push_back({a, b});
    if(a > b){
      starts.push_back(a); finishes.push_back(m+1);
      starts.push_back(1); finishes.push_back(b+1);
    } else {
      starts.push_back(a); finishes.push_back(b+1);
    }
  }
  
  // for(auto& [s, f]: jedis){
  //   cout << s << " " << f << endl;
  // }
  // print(starts); print(finishes);
  sort(starts.begin(), starts.end());
  sort(finishes.begin(), finishes.end());
  
  // print(starts); print(finishes);
  
  
  long starting_segment = find_segment_with_fewest_overlaps(starts, finishes);
  vector<long> possible_starts(1, starting_segment);
  // cout << "starting segment: " << starting_segment << endl; 
  for(auto& [s, f]: jedis){
    if(f < s) f+=m;
    if(s < starting_segment && f >= starting_segment){
      possible_starts.push_back(f);
      s = -1;
    } else if(s < starting_segment){
      s += m; f += m;
    }
  }
  
  sort(jedis.begin(), jedis.end(), 
    [](const pair<long, long>& jedi1, const pair<long, long>& jedi2) ->
      bool {
        return jedi1.second < jedi2. second;
      }
  );
  
  // for(auto& [s, f]: jedis){
  //   cout << s << " " << f << endl;
  // }
  
  int best_count = 0;
  for(size_t i = 0; i < possible_starts.size(); i++){
    int count = 1;
    if(i == 0) count--;
    long left = possible_starts[i];
    for(auto& [s, f]: jedis){
      if(s > left){
        left = f; count++;
      }
    }
    best_count = max(best_count, count);
  }
  cout << best_count << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
