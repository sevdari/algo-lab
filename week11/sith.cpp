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

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

int explore(int s, const vector<vector<int>>& neighs){
  vector<bool> is_visited(neighs.size(), false);
  priority_queue<int> q;
  q.push(s);
  is_visited[s] = true;
  int count = 0, min_el = s;
  while(q.size()){
    int curr = q.top(); q.pop();
    min_el = min(min_el, curr);
    if(min_el <= count) return count;
    count++;
    for(auto neigh: neighs[curr]){
      if(!is_visited[neigh]){
        is_visited[neigh] = true;
        q.push(neigh);
      }
    }
  }
  return count;
}

void testcase() {
  long n, r; cin>>n>>r;
  K::FT rr = (K::FT) r * (K::FT) r;
  typedef std::pair<K::Point_2,Index> IPoint;
  std::vector<IPoint> points;
  points.reserve(n); 
  
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay t;
  t.insert(points.begin(), points.end());
  
  EdgeV edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) std::swap(i1, i2);
    auto edge_length = t.segment(e).squared_length();
    if(edge_length > rr) continue;
    edges.emplace_back(i1, i2, edge_length);
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });
  
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  // ... and process edges in order of increasing length
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // determine components of endpoints
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2) {
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
      if (--n_components == 1) break;
    }
  }
  
  // uf.compress_sets();
  
  vector<vector<int>> components(n);
  
  for(int i = 0; i < n; i++)
    components[uf.find_set(i)].push_back(i);
  
  vector<vector<int>> neighs(n);
  for(int i = 0; i < n; i++){
    if(components[i].empty()) continue;
    for(int j = 0; j < components[i].size(); j++){
      for(int k = j + 1; k < components[i].size(); k++){
        int p1 = components[i][j], p2 = components[i][k];
        if(CGAL::squared_distance(points[p1].first, points[p2].first) <= rr){
          neighs[p1].push_back(p2); neighs[p2].push_back(p1);
        }
      }
    }
  }
  
  // for(int i = 0; i < n; i++){
  //   cout << i << ": ";
  //   for(auto neigh: neighs[i]){
  //     cout << neigh << " ";
  //   }
  //   cout << endl;
  // }
  
  int best = 0;
  for(int i = n-1; i>=0; i--){
    best = max(best, explore(i, neighs));
  }
  
  cout << best << endl;
    
}

int main() {
  std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
