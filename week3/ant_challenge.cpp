// ************************************
// IN PROGRESS
// only passes the first two test cases
// ************************************

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <iostream>
using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property,
boost::property<boost::edge_weight_t, int>
> weighted_graph;

typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

void run_dijkstra(
  weighted_graph& G,
  int s,
  vector<int>& dist_map,
  vector<vertex_desc>& pred_map
  ){
      boost::dijkstra_shortest_paths(G, s,
      boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
      boost::get(boost::vertex_index, G)))
      .predecessor_map(boost::make_iterator_property_map(pred_map.begin(),
      boost::get(boost::vertex_index, G))));
  }

void contruct_shortest_path_tree(
  weighted_graph& private_G, 
  weighted_graph& public_G,
  int hive
  ){
  int n = boost::num_vertices(private_G);
  vector<int> dist_map(n);
  vector<vertex_desc> pred_map(n);
  
  run_dijkstra(private_G, hive, dist_map, pred_map);
  
  // get edges in shortest paths tree
  auto weightmap_public = boost::get(boost::edge_weight, public_G);

  for (int tree = 0; tree < n; ++tree) {
      if (tree == hive) continue;
  
      int pred = pred_map[tree];
      auto private_edge = boost::edge(pred, tree, private_G).first;
      int private_w = boost::get(boost::edge_weight, private_G, private_edge);
  
      auto [public_edge, exists] = boost::edge(pred, tree, public_G);
      if (exists) {
          int public_w = boost::get(boost::edge_weight, public_G, public_edge);
          if (private_w < public_w)
              boost::put(weightmap_public, public_edge, private_w);
      } else {
          auto [new_edge, inserted] = boost::add_edge(pred, tree, public_G);
          boost::put(weightmap_public, new_edge, private_w);
      }
  }
}

void testcase(){
  int n, e, s, a, b; cin>>n>>e>>s>>a>>b;
  vector<weighted_graph> private_networks(s, weighted_graph(n));
  weighted_graph public_G(n);
  
  // read edges and construct graphs
  while(e--){
    int tree_1, tree_2; cin>>tree_1>>tree_2;
    for(int i = 0; i < s; i++){
      int weight; cin >> weight;
      boost::add_edge(tree_1, tree_2, weight, private_networks[i]);
    }
  }
  
  // read hives
  for(int i = 0; i < s; i++){
    int hive; cin>>hive;
    contruct_shortest_path_tree(private_networks[i], public_G, hive);
  }
  
  vector<int> dist_map(n);
  vector<vertex_desc> pred_map(n);
  run_dijkstra(public_G, a, dist_map, pred_map);
  cout<<dist_map[b]<<endl;
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int tests; cin>>tests;
  while(tests--){
    testcase();
  }
}