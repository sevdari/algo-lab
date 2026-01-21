///3
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Locate_type Locate_type;

Triangulation t;
long r; 
K::FT rr;
K::FT four = K::FT(4);

void bfs(Triangulation::Face_handle start){
  priority_queue<pair<K::FT,Triangulation::Face_handle>> q;
  q.push({-1, start});
  while(q.size()){
    auto current_face = q.top().second; q.pop();
    for(int i = 0; i < 3; i++){
      Triangulation::Face_handle neigh = current_face->neighbor(i);
      if(t.is_infinite(neigh)) continue;
      K::FT passage = CGAL::squared_distance(
        current_face->vertex((i+1)%3)->point(), current_face->vertex((i+2)%3)->point()
      );
      
      if(!t.is_infinite(current_face))
        passage = min(passage, current_face->info());
      if(passage > neigh -> info()){
        neigh -> info() = passage;
        q.push({passage, neigh});
      }
    }
  }
}

void testcase() {
  t.clear();
  int n, m; cin>>n>>m;
  cin>>r;
  rr = K::FT(r) * K::FT(r);
  vector<K::Point_2> trees, ballons;
  vector<long> ballon_radius(m);
  
  long x, y;
  for(int i = 0; i < n; i++){
    cin>>x>>y;
    trees.push_back(K::Point_2(x, y));
  }
  
  for(int i = 0; i < m; i++){
    cin>>x>>y>>ballon_radius[i];
    ballons.push_back(K::Point_2(x, y));
  }
  
  t.insert(trees.begin(), trees.end());
  
  // precompute
  for (Triangulation::Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
    f->info() = -1;
  
  Triangulation::Face_circulator f = t.incident_faces(t.infinite_vertex());
  do {
    bfs(f);
  } while (++f != t.incident_faces(t.infinite_vertex()));
  
  // for (Triangulation::Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
  //   cout << f -> info() << endl;
  
  // processing ballons
  for(int i = 0; i < m; i++){
    auto ballon = ballons[i];
    
    
    auto nearest_v = t.nearest_vertex(ballon);
    K::FT dist = CGAL::squared_distance(nearest_v -> point(), ballon);
    
    // check if too close to some tree
    K::FT need = r + ballon_radius[i];
    if(dist < need * need){ cout << "n"; continue; }
    
    // check take off clearance
    need = r + ballon_radius[i];
    if(dist >= four * need * need){ cout << "y"; continue; }
    
    // see if we can escape
    Locate_type lt; int li;
    auto fh = t.locate(ballon, lt, li);
    if(t.is_infinite(fh)){ cout << "y"; continue; }
    bool can_escape = four * need * need <= fh->info();
    if(lt==Locate_type::EDGE)
      can_escape = can_escape || four * need * need <= fh->neighbor(li)->info();
    cout << (can_escape ? "y" : "n");
      
  }
  cout << endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
