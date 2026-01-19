#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

void testcase() {
  // read input
  int n; cin>>n;
  vector<int> menus(n), cost(n), students(n), prices(n), freezer(n-1), energy_cost(n-1);
  int total_students = 0, MAX_PRICE = 20;
  
  for(int i = 0; i < n; i++)
    cin >> menus[i] >> cost[i];
  
  for(int i = 0; i < n; i++){
    cin >> students[i] >> prices[i];
    total_students += students[i];
  }
  
  for(int i = 0; i < n - 1; i++)
    cin >> freezer[i] >> energy_cost[i];
  
  // solution
  graph G(n);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  // the crux of the problem
  for(int i = 0; i < n; i++){
    adder.add_edge(v_source, i, menus[i], cost[i]);
    if(i != n - 1) adder.add_edge(i, i+1, freezer[i], energy_cost[i]);
    adder.add_edge(i, v_sink, students[i], MAX_PRICE-prices[i]);
  }
  
  // find flow and cost
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int profit = -boost::find_flow_cost(G);
  
  int flow = 0;
  out_edge_it e, eend;

  for (boost::tie(e, eend) = boost::out_edges(boost::vertex(v_sink,G), G); e != eend; ++e) {
    profit += (c_map[r_map[*e]] - rc_map[r_map[*e]]) * MAX_PRICE;
    flow += rc_map[*e] - c_map[*e];  
  }
  cout << (flow==total_students ? "possible " : "impossible ");
  cout << flow << " " << profit << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
