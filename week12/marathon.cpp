#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};


// BGL includes
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc_d;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc_d;

void dijkstra_dist(const weighted_graph &G, int s, vector<int>& dist_map) {

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
}

void testcase() {
  int n, m, s, f; cin>>n>>m>>s>>f;

  vector<vector<int>> lengths(n, vector<int>(n, 0));
  vector<vector<int>> widths(n, vector<int>(n, 0));

  for(int i = 0; i < m; i++){
    int u, v, width, length; cin>>u>>v>>width>>length;
    if(u==v) continue;
    if(lengths[u][v]==0 || length < lengths[u][v]){
        lengths[u][v] = length; lengths[v][u] = length;
        widths[u][v] = width; widths[v][u] = width;
    }else if(length == lengths[u][v]){
      widths[u][v] += width; widths[v][u] += width;
    }
  }
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc_d e;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(i==j) continue;
      if(lengths[i][j] > 0){
        e = boost::add_edge(i, j, G).first; weights[e]=lengths[i][j];
      }
    }
  }
  
  vector<int> dist_from_s(n), dist_to_f(n);
  dijkstra_dist(G, s, dist_from_s); 
  dijkstra_dist(G, f, dist_to_f);
  
  int shortest_path = dist_from_s[f];
  
  graph G2(n);
  edge_adder adder(G2);
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(i == j) continue;
      if(dist_from_s[i] + dist_to_f[j] + lengths[i][j] == shortest_path){
        adder.add_edge(i, j, widths[i][j]);
      }
    }
  }
  
  long flow = boost::push_relabel_max_flow(G2, s, f);
  std::cout << flow << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
