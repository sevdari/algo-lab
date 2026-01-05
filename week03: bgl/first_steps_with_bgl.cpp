#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int furthest_distance(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

    int res = 0;

    for (int d : dist_map) {
      if (d == std::numeric_limits<int>::max())
        continue;

      res = max(res, d);
    }
    
    return res;
}

void testcase() {
  int n, m; cin>>n>>m;
  weighted_graph G(4);
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;
  for(int i = 0; i < m; i++){
    int u, v, w; cin>>u>>v>>w;
    e = boost::add_edge(u, v, G).first; weights[e]=w;
  }
  std::vector<edge_desc> mst;  
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  int total = 0;
  for (auto e : mst)
    total += get(boost::edge_weight, G, e);
  cout << total << " " << furthest_distance(G, 0) << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
