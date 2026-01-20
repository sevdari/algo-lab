#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

void maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  
  cout << matching_size << endl;
}

void dijkstra_path(const weighted_graph &G, int s, vector<bool>& reachable, int d) {
  int n = boost::num_vertices(G);
  std::vector<int>         dist_map(n);
  std::vector<vertex_desc> pred_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G)))
    .predecessor_map(boost::make_iterator_property_map(
      pred_map.begin(), boost::get(boost::vertex_index, G))));

  for(int i = 0; i < n; i++)
    if (dist_map[i] <= d) reachable[i] = true;
  
}


void testcase() {
  long n, m, b, p, d; cin>>n>>m>>b>>p>>d;
  vector<int> barracks(b), plaza_ind(n, -1);
  for(int i = 0; i < b; i++)
    cin>>barracks[i];
  
  for(int i = 0; i < p; i++){
    int plaza; cin>>plaza;
    plaza_ind[plaza] = n + i;
  }
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  vector<pair<int, int>> roads;
  vector<int> lengths;
  edge_desc e;
  vector<bool> reachable(n, false);
  for(int i = 0; i < m; i++){
    int u, v, w; cin>>u>>v>>w;
    e = boost::add_edge(u, v, G).first; weights[e]=w;
    e = boost::add_edge(v, u, G).first; weights[e]=w;
    roads.push_back({u, v}); lengths.push_back(w);
  }
  
  for(auto barrack: barracks)
    dijkstra_path(G, barrack, reachable, d);
  
  graph G2(n + p);
  for(int i = 0; i < m; i++){
    auto [u, v] = roads[i];
    if(!reachable[u] || !reachable[v]) continue;
    boost::add_edge(u, v, G2);
    if(plaza_ind[u]!=-1) boost::add_edge(plaza_ind[u], v, G2);
    if(plaza_ind[v]!=-1) boost::add_edge(u, plaza_ind[v], G2);
  }
  
  maximum_matching(G2);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
