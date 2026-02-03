#include<bits/stdc++.h>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

bool contains(const vector<P>& map, const S& leg){
  bool flag = true;
  for(int j = 0; j < 6; j+=2){
    flag = flag & !CGAL::right_turn(map[j], map[j+1], leg.source());
    flag = flag & !CGAL::right_turn(map[j], map[j+1], leg.target());
  }
  return flag;
}

void testcase(){
  int n, m; cin>>m>>n;
  P last, curr;
  vector<S> legs;
  int x, y;
  for(int i = 0; i < m; i++){
    cin>>x>>y;
    curr = P(x, y);
    if(i != 0) legs.push_back(S(last, curr));
    last = curr;
  }
  
  vector<vector<P>> maps(n);
  for(int i = 0; i < n; i++){
    for(int j = 0; j < 6; j++){
      cin>>x>>y;
      maps[i].push_back(P(x, y));
    }
    
    for(int j = 0; j < 6; j += 2){
      if(!CGAL::left_turn(maps[i][j], maps[i][j+1], maps[i][(j+2)%6])){
        P temp = maps[i][j];
        maps[i][j] = maps[i][j+1];
        maps[i][j+1] = temp;
      }
    }
  }
  
  m--;
  vector<vector<bool>> contain(n, vector<bool>(m, false));
  
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      contain[i][j] = contains(maps[i], legs[j]);
    }
  }
  
  int count = 0;
  int best = n;
  vector<int> freq(m, 0);
  
  int i = 0, j = -1;

  while (j < n) {
    if (count < m) {
      j++;
      if (j == n) break;
      for(int k = 0; k < m; k++){
        if(contain[j][k]){
          freq[k] += 1;
          if(freq[k]==1) count += 1;
        }
      }
    } else {
      best = min(best, j - i + 1);
      for(int k = 0; k < m; k++){
        if(contain[i][k]){
          freq[k] -= 1;
          if(freq[k]==0) count -= 1;
        }
      }
      i++;
      if (i > j) {
        if (i == n) break;
        j = i - 1;
        count = 0; 
        for(int k = 0; k < m; k++) freq[k] = 0;
      }
    }
  }
  
  cout << best << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}