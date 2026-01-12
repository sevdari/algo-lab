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
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt SK;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;

// global vars
Triangulation tri;
vector<K::Point_2> pts;
long l, b, r, t; 

double ceil_to_double(SK::FT const & x){
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

long squared_distance_to_time(SK::FT squared_distance){
  SK::FT d = CGAL::sqrt(squared_distance);
  if (d <= SK::FT(0.5)) return 0;
  SK::FT death_time = CGAL::sqrt(d - SK::FT(0.5));
  return ceil_to_double(death_time);
}

K::FT calc_death_time(Vertex_iterator v){
  K::Point_2 current_point = v->point();
  // boundaries of the dish
  K::FT min_dist = CGAL::squared_distance(current_point, K::Point_2(l, current_point.y()));
  min_dist = min(min_dist, CGAL::squared_distance(current_point, K::Point_2(r, current_point.y())));
  min_dist = min(min_dist, CGAL::squared_distance(current_point, K::Point_2(current_point.x(), b)));
  min_dist = min(min_dist, CGAL::squared_distance(current_point, K::Point_2(current_point.x(), t)));

  // incident Vertices
  Triangulation::Vertex_circulator c = tri.incident_vertices(v);
  do {
    if(c==0) break;
    if (!tri.is_infinite(c)){
      min_dist = min(min_dist, CGAL::squared_distance(c->point(), current_point) / 4);
    }
  } while (++c != tri.incident_vertices(v));
  
  return min_dist;
}

void testcase(int n) {
  // input
  cin>>l>>b>>r>>t;
  tri.clear(); pts.clear();
  int x, y;
  for(int i = 0; i < n; i++){
    cin>>x>>y;
    pts.push_back(K::Point_2(x, y));
  }
  
  // solution
  tri.insert(pts.begin(), pts.end());
  vector<K::FT> death_time;
  
  for (Vertex_iterator v = tri.finite_vertices_begin(); v != tri.finite_vertices_end(); ++v){
    death_time.push_back(calc_death_time(v));
  }
  
  sort(death_time.begin(), death_time.end());
  
  // output
  cout << squared_distance_to_time(death_time[0]) << " "
       << squared_distance_to_time(death_time[n / 2]) << " "
       << squared_distance_to_time(death_time[n - 1]) << endl;
  
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