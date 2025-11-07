// **************************************
// I misread the problem as I thought
// multiple triangles could cover a leg.
// This is that solution.
// **************************************

#include <iostream>
using namespace std;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;
typedef K::Triangle_2 T;
typedef CGAL::Polygon_2<K> Poly;

P get_intersection(const R &ray1, const R &ray2){
  // assuming all rays intersect and dont overlap
  auto o = CGAL::intersection(ray1, ray2);
  const P* op = boost::get<P>(&*o);
  return *op;
}

T get_triangle(const vector<R> &rays){
  P vertex1 = get_intersection(rays[0], rays[1]);
  P vertex2 = get_intersection(rays[0], rays[2]);
  P vertex3 = get_intersection(rays[1], rays[2]);
  return T(vertex1, vertex2, vertex3);
}

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

void get_triangles(vector<T> &triangles, int n){
  for(int i = 0; i < n; i++){
    vector<R> rays;
    for(int j = 0; j < 3; j++){
      long q0, q1, q2, q3; cin>>q0>>q1>>q2>>q3;
      rays.push_back(R(P(q0, q1), P(q2, q3)));
    }
    triangles.push_back(get_triangle(rays));
  }
}

void add_new_legs(const S &original, const S &cut, vector<S> &new_legs){
  K::FT dist_to_source = CGAL::squared_distance(original.source(),cut.source());
  K::FT dist_to_target = CGAL::squared_distance(original.source(),cut.target());

  P closest_to_source = dist_to_source < dist_to_target ? cut.source() : cut.target();
  P closest_to_target = dist_to_source < dist_to_target ? cut.target() : cut.source();
  
  if(closest_to_source!=original.source()){
    new_legs.push_back(S(original.source(), closest_to_source));
  }
  
  if(closest_to_target!=original.target()){
    new_legs.push_back(S(original.target(), closest_to_target));
  }

}

void reduce_legs(vector<S> &legs, T triangle){
  vector<S> new_legs;
  for(int i = legs.size() - 1; i >= 0; i--){
    S leg = legs[i];
    if (CGAL::do_intersect(leg,triangle)) {
      auto o = CGAL::intersection(leg, triangle);
      if (const P* op = boost::get<P>(&*o)){
        continue;
      } else if (const S* os = boost::get<S>(&*o)) {
        legs.erase(legs.begin() + i);
        add_new_legs(leg, *os, new_legs);
      }
    }
  }
}

void testcase(){
  int m, n; cin>>m>>n;
  // legs
  vector<S> legs;
  get_legs(legs, m);
  // triangles
  vector<T> triangles;
  get_triangles(triangles, n);
  
  for(int i = n-1; i>=0; i--){
    reduce_legs(legs, triangles[i]);
    if(legs.size()==0){
      cout << n - i << endl;
      break;
    }
  }
}

int main(){
  ios_base::sync_with_stdio(false);
  int tests; cin>>tests;
  while(tests--){
    testcase();
  }
}