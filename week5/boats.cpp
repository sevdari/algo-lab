#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

typedef long long ll;
const ll N = 2e5;

vector<pair<ll, ll>> boats(N);

ll find_best_pos(ll left_threshold, ll l, ll pos, ll right_threshold){
  if(left_threshold + l <= pos) return pos;
  if(left_threshold + l > right_threshold) return -1;
  return left_threshold + l;
}

void testcase(){
  long n; cin>>n;
  for(int i = 0; i < n; i++){
    ll l, p; cin>>l>>p;
    boats[i] = {l, p};
  }
  sort(boats.begin(), boats.begin() + n,
     [](const auto& a, const auto& b){
         if (a.second != b.second) return a.second < b.second; 
         return a.first < b.first;            
     });
    
  vector<ll> res;
  for(int i = 0; i < n; i++){
    auto boat = boats[i];
    ll l = boat.first, p = boat.second;
    
    // base cases
    if(i == 0){ res.push_back(p); continue;}
    if(i == n- 1){ res.push_back(p + l); continue; }
    
    // actual logic
    if(p < res.back()){
      ll best_p = find_best_pos(*(res.end()-2), l, p, boats[n-1].second);
      if(best_p != -1 && best_p < res.back()){
        res.pop_back();
        res.push_back(best_p);
      }
    } else {
      ll best_p = find_best_pos(*(res.end()-1), l, p, boats[n-1].second);
      if(best_p != -1) res.push_back(best_p);
    }
  }

  
  cout << res.size() << endl;
  
}

int main(){
  int t; cin>>t;
  while(t--){
    testcase();
  }
}