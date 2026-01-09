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
  long n, m, D, T; cin>>n>>m>>D>>T;
  vector<pair<long, long>> moves;
  
  for(long i = 0; i < n; i++){
    long d, t; cin>>d>>t;
    moves.push_back({d, t});
  }
  
  vector<long> potions(m);
  for(long i = 0; i < m; i++){
    cin>>potions[i];
  }
  
  // solution
  long best = m + 1;
  
  // run through all subsets
  for (long s = 0; s < 1<<n; ++s) { 
    long d = 0, t = 0, count = 0;
    for (long i = 0; i < n; ++i) {
      if (s & 1<<i){
        d += moves[i].first;
        t += moves[i].second;
        count++;
      }
    }
    // best case scenario
    if(d >= D and t < T){
      cout << 0 << endl; return;
    }
    
    // can we get there by potions?
    if(count == 0 || m == 0) continue;
    long need = (long)ceil((double)(D - d) / count);
    auto it = lower_bound(potions.begin(), potions.end(), need);
    if(it != potions.end())
      best = min(best, (long)(it - potions.begin()) + 1);
  }
  
  if(best == m + 1)
    cout << "Panoramix captured\n";
  else
    cout << best << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  long t;
  std::cin >> t;
  for (long i = 0; i < t; ++i)
    testcase();
}
