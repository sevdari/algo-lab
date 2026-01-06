#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Ray_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Segment_2< K > Segment;
typedef CGAL::Point_2< K >  Point;
typedef CGAL::Ray_2< K > Ray;

void testcase(int n) {
  long x, y, a, b; cin>>x>>y>>a>>b;
  Ray photon = Ray(Point(x, y), Point(a, b));
  vector<Segment> walls;
  for(int i = 0; i < n; i++){
    long r, s, t, u; cin>>r>>s>>t>>u;
    Segment wall = Segment(Point(r, s), Point(t, u));
    walls.push_back(wall);
  }
  
  for(auto wall: walls){
    if (CGAL::do_intersect(photon, wall)){
      cout << "yes\n"; return;
    } 
  }
  cout << "no\n";
  
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;
  while(n!=0){
    testcase(n); cin>>n;
  }
}