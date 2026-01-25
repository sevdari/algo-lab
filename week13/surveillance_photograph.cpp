///2
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

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

void testcase() {
  int n, m, k, l; cin>>n>>m>>k>>l;
  vector<int> stations(n, 0), photos(n, 0);
  long res = 0;
  for(int i = 0; i < k; i++){
    int station_id; cin>>station_id;
    stations[station_id]++;
  }
  
  for(int i = 0; i < l; i++){
    int photo_id; cin>>photo_id;
    if(stations[photo_id] > 0){
      res++; stations[photo_id]--; continue;
    }
    photos[photo_id]++;
  }
  
  vector<vector<int>> roads(n, vector<int>(n, 0));
  for(int i = 0; i < m; i++){
    int u, v; cin>>u>>v;
    roads[u][v] += 1;
  }
  
  graph G(2*n);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for(int i = 0; i < n; i ++){
    if(photos[i] > 0){
      adder.add_edge(i, n+i, photos[i]);
    }
    if(stations[i] > 0){
      adder.add_edge(v_source, i, stations[i]);
      adder.add_edge(n+i, v_sink, stations[i]);
    }
  }
  
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(roads[i][j] > 0){
        adder.add_edge(n+i, n+j, roads[i][j]);
        adder.add_edge(i, j, k);
      }
    }
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << res + flow << "\n";

}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
