/*
Still has a bug
*/
#include <iostream>
using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

void get_legs(vector<S> &legs, int m){
  long last_x = -1, last_y = -1;
  for(int i = 0; i < m; i++){
    long x, y; cin>>x>>y;
    if(last_x != -1){
      legs.push_back(S(P(last_x, last_y), P(x, y)));
    }
    last_x = x;
    last_y = y;
  }
}

void get_triangle(vector<P> &triangle){
  for(int i = 0; i < 6; i++){
    long q0, q1; cin>>q0>>q1;
    triangle.push_back(P(q0, q1));
  }
  for(int j = 0; j < 6; j += 2){
    if (CGAL::right_turn(triangle[j],triangle[j+1],triangle[(j+2)%6])) swap(triangle[j],triangle[j+1]);
  }
}

bool contains_point(const vector<P> &triangle, const P &p){
  return (
     !CGAL::right_turn(triangle[0], triangle[1], p) &&
     !CGAL::right_turn(triangle[2], triangle[3], p) &&
     !CGAL::right_turn(triangle[4], triangle[5], p)
  );
}

bool contains_leg(vector<P> &triangle, S &leg){
  return contains_point(triangle, leg.source()) && 
         contains_point(triangle, leg.target());
}

void testcase(){
  int m, n; cin>>m>>n;
  vector<vector<bool>> covered(n, vector<bool>(m-1, false));
  // legs
  vector<S> legs;
  get_legs(legs, m);
  // triangles
  for(int i = 0; i < n; i ++){
    vector<P> triangle;
    get_triangle(triangle);
    for(int j = 0; j < m - 1; j++){
      if(contains_leg(triangle, legs[j])){
        covered[i][j] = true;
      }
    }
  }
  // sliding window
  vector<int> current_cover(m-1, 0);
  int left = 0, right = 0;
  int min_cost = n;
  for(int j = 0; j < m - 1; j++)
    if(covered[0][j]) current_cover[j] += 1;
  while(right < n){
    bool flag = true;
    for(int j = 0; j < m-1; j++){
      if(current_cover[j] == 0) { flag = false; break;}
    }
    if(flag) {
      min_cost = min(min_cost, right - left + 1);
      for(int j = 0; j < m - 1; j++){
        if(covered[left][j]) current_cover[j] -= 1;
      }
      left += 1;
    } else {
      right += 1;
      if(right >= n) break;
      for(int j = 0; j < m - 1; j++){
        if(covered[right][j]) current_cover[j] += 1;
      }
    }
  }
  cout<<min_cost<<endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int tests; cin>>tests;
  while(tests--){
    testcase();
  }
}