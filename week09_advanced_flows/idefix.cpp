// doesnt solve the last two test cases

///1
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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef int                                                   Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>  Vb;
typedef CGAL::Triangulation_face_base_2<K>                    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>           Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                 Triangulation;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

std::ostream& operator<<(std::ostream& o, const Edge& e) {
  return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

Triangulation t;
int ncc;
long n, m, s, k;

void bfs(const Triangulation::Vertex_handle v){
  std::queue<Triangulation::Vertex_handle> q;
  q.push(v);
  while(q.size()){
    Triangulation::Vertex_handle curr = q.front(); q.pop();
    Triangulation::Vertex_circulator c = t.incident_vertices(curr);
    if(c==0) continue;
    do {
      if(c->info() != -1 || t.is_infinite(c)) continue;
      Triangulation::Vertex_handle vh = c;
      if(CGAL::squared_distance(vh->point(), curr->point()) > s) continue;
      vh -> info() = ncc; q.push(vh);
    } while (++c != t.incident_vertices(curr));
  }
}

void testcase() {
  ncc = 0; t.clear();
  cin>>n>>m>>s>>k;
  vector<K::Point_2> trees(n), bones(m);
  int x, y; 
  for(int i = 0; i < n; i++){
    cin>>x>>y;
    trees[i] = K::Point_2(x, y);
  }
  
  for(int i = 0; i < m; i++){
    cin>>x>>y;
    bones[i] = K::Point_2(x, y);
  }
  

  t.insert(trees.begin(), trees.end());
  
  for(Triangulation::Vertex_iterator v = t.finite_vertices_begin(); v!= t.finite_vertices_end(); v++)
    v -> info() = -1;
  
  
  for(Triangulation::Vertex_iterator v = t.finite_vertices_begin(); v!= t.finite_vertices_end(); v++){
    if(v -> info() != -1) continue;
    v -> info() = ncc;
    bfs(v);
    ncc++;
  }
  
  int a = 0;
  vector<int> bones_per_comp(ncc, 0);
  for(auto bone: bones){
    auto vh = t.nearest_vertex(bone);
    if (4 * CGAL::squared_distance(bone, vh->point()) > s) continue;
    bones_per_comp[vh->info()]++;
    a = max(a, bones_per_comp[vh->info()]);
  }
  
  // EdgeV edges;
  // edges.reserve(3*n); // there can be no more in a planar graph
  // for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
  //   Index i1 = e->first->vertex((e->second+1)%3)->info();
  //   Index i2 = e->first->vertex((e->second+2)%3)->info();
  //   // ensure smaller index comes first
  //   if (i1 > i2) std::swap(i1, i2);
  //   edges.emplace_back(i1, i2, t.segment(e).squared_length());
  // }
  // std::sort(edges.begin(), edges.end(),
  //     [](const Edge& e1, const Edge& e2) -> bool {
  //       return std::get<2>(e1) < std::get<2>(e2);
  //           });
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
