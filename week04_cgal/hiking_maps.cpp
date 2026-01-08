#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef IK::Point_2 P;
typedef IK::Segment_2 S;

int n, m;
vector<S> legs;
vector<vector<P>> triangles;

bool inside_triangle(const vector<P>& t, const P& p){
  return 
    !CGAL::right_turn(t[0], t[1], p) &&
    !CGAL::right_turn(t[2], t[3], p) &&
    !CGAL::right_turn(t[4], t[5], p);
}

bool inside_triangle(const vector<P>& t, const S& s){
  return 
    inside_triangle(t, s.source()) && inside_triangle(t, s.target());
}

void sliding_window(){
  int left = 0, right = 0, count = 0;
  vector<int> freq(m - 1, 0);
  int best = n;
  while(right < n){
    // process new triangle
    for(int i = 0; i < m - 1; i++){
      S leg = legs[i];
      if(inside_triangle(triangles[right], leg)){
        if(freq[i] == 0) count++;
        freq[i]++;
      }
    }
    // update best
    if(count==m - 1){
      best = min(right - left + 1, best);
      while(left < right && count == m - 1){
        for(int i = 0; i < m - 1; i++){
          S leg = legs[i];
          if(inside_triangle(triangles[left], leg)){
            freq[i]--;
            if(freq[i] == 0) count--;
          }
        }
        left ++;
        if(count==m - 1) best = min(right - left + 1, best);
      }
    }
    right++;
  }
  cout << best << endl;
}

void testcase() {
  legs.clear();
  triangles.clear();
  cin>>m>>n;
  
  P last;
  for(int i = 0; i < m; i++){
    int x, y; cin>>x>>y;
    P new_leg = P(x, y);
    if(i!=0)
      legs.push_back(S(last, new_leg));
    last = new_leg;
  }
  
  for(int i = 0; i < n; i++){
    vector<P> t;
    for(int j = 0; j < 6; j++){
      int x, y; cin>>x>>y;
      t.push_back(P(x, y));
    }
    for (std::size_t j = 0; j < 6; j+=2)
      if (CGAL::right_turn(t[j],t[j+1],t[(j+2)%6])) std::swap(t[j],t[j+1]);
    triangles.push_back(t);
  }
  
  sliding_window();
  
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}