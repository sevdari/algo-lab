/*
Needs Split and List to pass last test case
*/
#include<iostream>
#include<algorithm>
#include<vector>
#include <list>
#include<set>

using namespace std;

typedef long long ll;
const ll M = 100000;
const int N = 30;

vector<ll> d(M);
vector<ll> t(M);
vector<ll> gulps(M);

ll find_min_m(ll diff, int moves, ll m){
    if (m == 0 || moves == 0) return M+1;
    ll need = (diff + moves - 1) / moves;
    auto it = lower_bound(gulps.begin(), gulps.begin() + m, need);
    if (it == gulps.begin() + m) return M + 1;
    return it - gulps.begin() + 1;
}


ll subsets(int n, ll m, ll D, ll T){
  ll min_m = M + 1;
  for(int s=0; s < 1 << n; ++s){
    int moves = __builtin_popcount(s);
    ll curr_d = 0, curr_t = 0;
    for(int i=0; i<n; i++){
      if(s & 1<<i) {
        curr_d += d[i];
        curr_t += t[i];
      }
      if(curr_d >= D || curr_t > T) break;
    }
    if(curr_d >= D && curr_t < T) return 0;
    if(curr_t < T)
      min_m = min(min_m, find_min_m(D - curr_d, moves, m));
  }
  return min_m;
}

void testcase(){
  int n; cin>>n;
  ll m, D, T; cin>>m>>D>>T;
  for(int i = 0; i < n; i++){
    cin>>d[i]; cin>>t[i];
  }
  for(int i = 0; i < m; i++){
    cin>>gulps[i];
  };
  
  ll sol = subsets(n, m, D, T);
  if(sol == M+1) {
    cout << "Panoramix captured \n";
  } else {
    cout << sol << '\n';
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin>>t;
  while(t--){
    testcase();
  }
}