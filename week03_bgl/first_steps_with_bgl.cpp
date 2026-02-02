#include<bits/stdc++.h>

using namespace std;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


int kruskal(const weighted_graph &G) {
  std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  int total = 0;
  for (auto e : mst)
    total += get(boost::edge_weight, G, e);
  
  return total;
}

int dijkstra_path(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int>         dist_map(n);
  std::vector<vertex_desc> pred_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  int max_dist = -1;
  for(auto dist: dist_map)
    max_dist = max(max_dist, dist);
  
  return max_dist;
}


void testcase(){
  int n, m; cin>>n>>m;
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  int from, to, w;
  for(int i = 0; i < m; i++){
    cin>>from>>to>>w;
    edge_desc e;
    e = boost::add_edge(from, to, G).first; weights[e]=w;
  }
  
  cout << kruskal(G) << " " << dijkstra_path(G, 0) << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}