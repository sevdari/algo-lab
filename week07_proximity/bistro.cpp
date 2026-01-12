#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase(int n) {
  // input
  vector<K::Point_2> existing, new_locs;
  int x, y;
  for(int i = 0; i < n; i++){
    cin>>x>>y;
    existing.push_back(K::Point_2(x, y));
  }
  
  int m; cin>> m;
  for(int i = 0; i < m; i++){
    cin>>x>>y;
    new_locs.push_back(K::Point_2(x, y));
  }
  
  // solution
  Triangulation t;
  t.insert(existing.begin(), existing.end());
  
  for(auto loc: new_locs){
    K::Point_2 nearest = t.nearest_vertex(loc)->point();
    cout << CGAL::squared_distance(nearest, loc) << endl;
  }
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int n;
  std::cin >> n;
  while(n!=0){
    testcase(n); cin>>n;
  }
}