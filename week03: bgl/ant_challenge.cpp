#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

struct WeightedEdge { int u, v, w; };


int n, edges_count, species_count, a, b; 
vector<pair<int, int>> edges;
vector<vector<int>> time_per_edge;
vector<int> hives;

vector<WeightedEdge> get_prim_tree(int s){
  // construct graph
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc ed;
  for(int i = 0; i < edges_count; i++){
    auto [u, v] = edges[i];
    ed = boost::add_edge(u, v, G).first; weights[ed] = time_per_edge[i][s];
  }
  vector<vertex_desc> p(n);
  p[0] = hives[s];
  //
  boost::prim_minimum_spanning_tree(G, &p[0]);
  vector<WeightedEdge> mst;
  for (std::size_t i = 0; i != p.size(); ++i)
    if (p[i] != i){
        auto [ed, ok] = boost::edge(p[i], i, G);
        mst.push_back({p[i], i, weights[ed]});
    }
    
  return mst;
}


int dijkstra_dist(const weighted_graph &G, int a, int b) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, a,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[b];
}

void testcase() {
  // read input
  cin>>n>>edges_count>>species_count>>a>>b;
  edges.clear();
  time_per_edge.assign(edges_count, vector<int>(species_count));
  
  for(int i = 0; i < edges_count; i++){
    int u, v; cin>>u>>v; edges.push_back({u, v});
    for(int j = 0; j < species_count; j++){
      cin>>time_per_edge[i][j];
    }
  }
  for(int i = 0; i < species_count; i++){
    int hive; cin>>hive; hives.push_back(hive);
  }
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc ed;
  
  // start solution
  vector<WeightedEdge> mst;
  for(int s = 0; s < species_count; s++){
    mst = get_prim_tree(s);
    for(auto [u, v, w]: mst){
      auto [ed, exists] = boost::edge(u, v, G); 
      if(exists){
        weights[ed] = min(weights[ed], w);
      } else {
        ed = boost::add_edge(u, v, G).first; weights[ed] = w;
      }
    }
  }
  
  cout << dijkstra_dist(G, a, b) << endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
