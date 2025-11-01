#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <iostream>
using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property,
boost::property<boost::edge_weight_t, int>
> weighted_graph;

typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

void testcase(){
  int n, m; cin>>n>>m;
  weighted_graph G(n);
  for(int i = 0; i < m; i++){
    int u, v, w; cin>>u>>v>>w;
    boost::add_edge(u, v, w, G);
  }
  
  vector<edge_desc> mst; 
  vector<int> dist_map(n);
  
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  boost::dijkstra_shortest_paths(G, 0,
  boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
  boost::get(boost::vertex_index, G))));
  
  int max_dist = 0, sum_mst = 0;
  
  for(int dist: dist_map){
    max_dist = max(dist, max_dist);
  }
  
  for(vector<edge_desc>::iterator it = mst.begin(); it!=mst.end(); it++){
    sum_mst += boost::get(boost::edge_weight, G, *it);
  }
  
  cout<<sum_mst<<" "<<max_dist<<endl;
  
}

int main(){
  
  int tests; cin>>tests;
  while(tests--){
    testcase();
  }
}