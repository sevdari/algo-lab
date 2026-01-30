///2
#include<bits/stdc++.h>

using namespace std;

typedef std::pair<int, int> PII;

void testcase(){
  int n, m; cin>>n>>m;
  long a, b; cin>>a>>b;
  long P, H, W; cin>>P>>H>>W;
  
  vector<pair<long, long>> a_potion;
  long x, y;
  for(int i = 0; i < n; i++){
    cin>>x>>y;
    a_potion.push_back({x, y});
  }
  
  vector<long> b_potion(m);
  for(int i = 0; i <m; i++){
    cin>>b_potion[i];
  }
  
  vector<vector<vector<long>>> memo(n+1, 
      vector<vector<long>>(n+1, 
      vector<long>(H+1, LONG_MIN)
    ));
  
  vector<long> max_power_from_a_potions(n+1, LONG_MIN);
    
  // base case
  for(int j = 1; j <= n; j++){
    auto [power, happy] = a_potion[j-1];
    memo[1][j][0] = max(power, memo[1][j-1][0]);
    // cout << power << " " << happy << endl;
    for(int h = 1; h <= H; h++){
      long temp = happy >= h ? power : LONG_MIN;
      memo[1][j][h] = max(temp, memo[1][j-1][h]);
    }
    if(memo[1][j][H] >= 0)
      max_power_from_a_potions[1] = max(max_power_from_a_potions[1], memo[1][j][H]);
  }
  
  for(int count = 2; count <= n; count ++){
    for(int j = count; j <= n; j++){
      auto [power, happy] = a_potion[j-1];
      memo[count][j][0] = max(memo[count][j-1][0], memo[count-1][j-1][0] + power);
      for(int h = 1; h <= H; h++){
        memo[count][j][h] = max(
          memo[count][j-1][h], 
          memo[count-1][j-1][max(0L, h-happy)] + power
        );
      }
      if(memo[count][j][H] >= 0)
       max_power_from_a_potions[count] = max(max_power_from_a_potions[count], memo[count][j][H]);
    }
  }

  int best = 201;
  sort(b_potion.begin(), b_potion.end(), greater<long>());
  long wit = 0, power = 0;
  for(int i = 0; i < m; i++){
    wit += b_potion[i];
    if(wit < W) continue;
    power = (i + 1) * b;
    int max_a_potions;
    if(a == 0) max_a_potions = n + 1;
    else max_a_potions = (wit - W) / max(a, 1L);
    long power_needed = P + power;
    
    auto ind = lower_bound(max_power_from_a_potions.begin(), max_power_from_a_potions.end(), power_needed);
    if(ind!=max_power_from_a_potions.end()){
      int a_potions_needed = ind - max_power_from_a_potions.begin();
      if(a_potions_needed <= max_a_potions)
        best = min(a_potions_needed + i + 1, best);
    }
    
  }
  
  if(best!=201)
    cout << best << endl;
  else
    cout << "-1\n";
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
}