#include<iostream>
#include<vector>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

using namespace std;

void testcase(int n){
  vector<K::Point_2> pts;
  pts.reserve(n);
  for(int i = 0; i < n; i++){
    int x, y; cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  int m; cin>>m;
  while(m>0){
    m--;
    int x, y; cin>>x>>y;
    K::Point_2 new_loc = K::Point_2(x, y);
    K::Point_2 nearest = t.nearest_vertex(new_loc)->point();
    K::FT d = CGAL::squared_distance(new_loc,nearest);
    std::cout << d << "\n";
  }
}


int main(){
  ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  int n; cin>>n;
  while(n!=0){
    testcase(n);
    cin>>n;
  }
}