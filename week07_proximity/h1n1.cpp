#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Triangulation_vertex_base_2<K>                 Vb;
typedef CGAL::Triangulation_face_base_with_info_2<double, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>          Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Face_handle  Face_handle;

using namespace std;

Triangulation t;

void bfs(Triangulation::Face_handle start_face){
  priority_queue<pair<double, Triangulation::Face_handle>> q;
  q.push({start_face -> info(), start_face});
  while(!q.empty()){
    auto [_, current_face] = q.top();
    q.pop();
    for(int i = 0; i < 3; i++){
      Triangulation::Face_handle neigh = current_face->neighbor(i);
      if(t.is_infinite(neigh)) continue;
      
      K::Point_2 v1 = current_face->vertex((i + 1) % 3) -> point();
      K::Point_2 v2 = current_face->vertex((i + 2) % 3) -> point();
      double edge_dist = CGAL::squared_distance(v1, v2);
      double dist_to_prop = min(edge_dist, current_face->info());
      
      if(dist_to_prop > neigh->info()){
        neigh -> info() = dist_to_prop;
        q.push({neigh -> info(), neigh});
      }
    }
  }
}

void testcase(int n) {
  t.clear();
  // input
  long x, y, d;
  vector<K::Point_2> pts;
  for(int i = 0; i < n; i++){
    cin>>x>>y; pts.push_back(K::Point_2(x, y));
  }
  
  int m; cin>>m;
  vector<pair<K::Point_2, long>> balls;
  for(int i = 0; i < m; i++){
    cin>>x>>y>>d; balls.push_back({K::Point_2(x, y), d});
  }
  // start solution
  t.insert(pts.begin(), pts.end());
  
  for(auto f = t.all_faces_begin(); f!=t.all_faces_end(); f++){
    if(t.is_infinite(f))
      f -> info() = std::numeric_limits<double>::max();
    else
      f -> info() = 0;
  }
  
  // precompute maximum escape distances
  Triangulation::Face_circulator start = t.incident_faces(t.infinite_vertex());
  Triangulation::Face_circulator fc = start;
  do{
    bfs(fc);
    fc++;
  }while(fc!=start);
  
  // process query points
  for(int i = 0; i < m; i++){
    auto [point, r] = balls[i];
    
    // check if already too close to an infected person
    K::Point_2 nearest = t.nearest_vertex(point) -> point();
    if(CGAL::squared_distance(nearest, point) < r){
      cout << "n"; continue;
    }
    // check for escape
    Triangulation::Locate_type lt; int li;
    Face_handle f = t.locate(point, lt, li);
    if(r * 4 <= f -> info())
      cout << "y";
    else if(lt == Triangulation::EDGE && r * 4 <= f -> neighbor(li) -> info())
      cout << "y";
    else
      cout << "n";
  }
  cout << endl;
  
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
