// Only solves for b = 1

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
// BGL flow include
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class
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
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int n, b, simba, nala;
  cin >> n >> b >> simba >> nala;
  
  vector<long> a(n);
  for(int i = 0; i < n; i++) cin >> a[i];
  
  vector<vector<long>> f(n, vector<long>(n, 0));
  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
      cin >> f[i][j];
  
  const long INF = 1e18;
  long min_cost = LONG_MAX;
  
  // Try both: simba on team A, nala on team B AND vice versa
  for(int swap = 0; swap < 2; swap++) {
    int on_A = (swap == 0) ? simba : nala;  // forced to team A
    int on_B = (swap == 0) ? nala : simba;  // forced to team B
    
    int source = n;   // represents team A
    int sink = n + 1; // represents team B
    
    graph G(n + 2);
    edge_adder adder(G);
    
    // Force on_A to team A, on_B to team B
    adder.add_edge(source, on_A, INF);
    adder.add_edge(on_B, sink, INF);
    
    // Add affinity edges for all cubs
    for(int i = 0; i < n; i++) {
      if(a[i] > 0) {
        adder.add_edge(source, i, a[i]);
      } else if(a[i] < 0) {
        adder.add_edge(i, sink, -a[i]);
      }
    }
    
    // Add friendship edges (undirected)
    for(int i = 0; i < n; i++) {
      for(int j = i + 1; j < n; j++) {
        if(f[i][j] > 0) {
          adder.add_edge(i, j, f[i][j]);
          adder.add_edge(j, i, f[i][j]);
        }
      }
    }
    
    long flow = boost::push_relabel_max_flow(G, source, sink);
    min_cost = min(min_cost, flow);
  }
  
  cout << min_cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cout << fixed << setprecision(0);
  int t;
  cin >> t;
  for(int i = 0; i < t; ++i)
    testcase();
}