///3
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
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;

typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::Vertex_iterator  Vertex_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS    
    >          graph;


void testcase() {
  // input
  long n, m, r; cin>>n>>m>>r;
  long r_squared = r * r;
  vector<K::Point_2> stations;
  vector<pair<K::Point_2, K::Point_2>> clues;
  int x, y;
  for(int i = 0; i < n; i++){
    cin>>x>>y;
    stations.push_back(K::Point_2(x, y));
  }
  for(int i = 0; i < m; i++){
    cin>>x>>y; K::Point_2 p(x, y);
    cin>>x>>y; K::Point_2 q(x, y);
    clues.push_back({p, q});
  }
  
  Triangulation t;
  t.insert(stations.begin(), stations.end());
  
  int ind = 0;
  for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
     v -> info() = ind; ind++;
  }
  
  graph G(stations.size()); // to use for connected components
  
  for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
    int count = 0;
    Triangulation::Vertex_circulator c = t.incident_vertices(v);
    do {
      if(c==0) break;
      if (!t.is_infinite(c)) {
        if(CGAL::squared_distance(v->point(), c->point()) <= r_squared){
          count++;
          boost::add_edge(v->info(), c->info(), G);
          if(count>=2){
            cout << "hit" << endl;
            for(size_t i = 0; i < clues.size(); i++){
              cout << "n"; 
            }
            cout << endl;
            return;
          }
        }
      }
    } while (++c != t.incident_vertices(v));
  }
  
  std::vector<int> component_map(n); 
  int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 
  
  for(auto [a, b]: clues){
    if(CGAL::squared_distance(a, b) <= r_squared){
      cout << "y"; continue;
    }
    Vertex_handle nearest_a = t.nearest_vertex(a);
    Vertex_handle nearest_b = t.nearest_vertex(a);
    if(CGAL::squared_distance(a, nearest_a->point()) > r_squared 
    || CGAL::squared_distance(b, nearest_b->point()) > r_squared){
      cout << "n"; continue;
    }
    
    if(component_map[nearest_a->info()]==component_map[nearest_b->info()]){
      cout <<"y"; continue;
    }
    cout <<"n"; continue;
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
