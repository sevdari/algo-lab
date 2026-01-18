///2
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


void testcase() {
  int V, E; cin>>V>>E;
  long total_demand=0;
  // graph
  graph G(V);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  // read supply demand
  vector<int> supply(V), demand(V);
  for(int i = 0; i < V; i++){
    cin>>supply[i]>>demand[i];
    adder.add_edge(i, v_sink, demand[i]);
    total_demand += demand[i];
  }
  // read edges
  for(int i = 0; i < E; i++){
    int s, t, c_min, c_max; cin>>s>>t>>c_min>>c_max;
    adder.add_edge(s, t, c_max - c_min);
    supply[s] -= c_min;
    supply[t] += c_min;
  }
  
  for(int i = 0; i < V; i++){
    adder.add_edge(v_source, i, supply[i]);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  cout << (flow==total_demand ? "yes\n" : "no\n");

}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
