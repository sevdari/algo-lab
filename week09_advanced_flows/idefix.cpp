///1
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

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

std::ostream& operator<<(std::ostream& o, const Edge& e) {
  return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

typedef K::Point_2 Point;
typedef std::pair<K::Point_2,Index> IPoint;

void testcase() {
  long n, m, s, k; cin>>n>>m>>s>>k;
  vector<IPoint> trees, bones;
  int x, y;
  for(Index i = 0; i < n; i++){
    cin>>x>>y; trees.push_back({Point(x, y), i});
  }
  for(Index i = 0; i < m; i++){
    cin>>x>>y; bones.push_back({Point(x, y), n+i});
  }
  
  // find a
  Delaunay t;
  t.insert(trees.begin(), trees.end());
  
  EdgeV edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });

  // Compute EMST using Kruskal's algorithm. This step takes O(n alpha(n)) time
  // in theory; for all practical purposes alpha(n) is constant, so linear time.

  // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  // ... and process edges in order of increasing length
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    
    if(std::get<2>(*e) > s){
      break;
    }
    
    // determine components of endpoints
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2) {
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
      if (--n_components == 1) break;
    }
  }
  
  vector<int> components(n, 0);
  int best_a = 0;
  for(auto [bone, idx]: bones){
    auto nearest = t.nearest_vertex(bone);
    if(4 * CGAL::squared_distance(bone, nearest->point()) > s) continue;
    int comp = uf.find_set(nearest -> info());
    components[comp] += 1;
    best_a = max(best_a, components[comp]);
  }
  
  
  // find q
  for(int i = 0; i < m; ++i) {
    // Find Edge for the bone i
    auto vertex_handle = t.nearest_vertex(bones[i].first);
    edges.emplace_back(n + i, vertex_handle->info(), 4 * CGAL::squared_distance(bones[i].first, vertex_handle->point()));
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });
  
  n_components = n + m;
  K::FT best_q = 0;
  vector<int> bone_count(n+m, 0);
  for(int i = n; i < n + m; i++)
    bone_count[i] = 1;
  boost::disjoint_sets_with_storage<> uf2(n+m);
  // ... and process edges in order of increasing length
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    
    // determine components of endpoints
    Index c1 = uf2.find_set(std::get<0>(*e));
    Index c2 = uf2.find_set(std::get<1>(*e));
    if (c1 != c2) {
      // this edge connects two different components => part of the emst
      uf2.link(c1, c2);
      Index c3 = uf2.find_set(std::get<0>(*e));
      int total_bones = bone_count[c1] + bone_count[c2];
      bone_count[c1] = 0;
      bone_count[c2] = 0;
      bone_count[c3] = total_bones;
      if(bone_count[c3] >= k){
        // cout << "Condition hit\n";
        best_q = std::get<2>(*e);
        break;
      }
      if (--n_components == 1) break;
    }
  }
  
  cout << best_a << " " << best_q << endl;
  
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
