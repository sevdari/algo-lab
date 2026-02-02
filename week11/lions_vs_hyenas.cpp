#include<bits/stdc++.h>

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

void testcase(){
  int n, b, k, m; cin>>n>>b>>k>>m;
  vector<int> aff(n);
  
  long total = 0;
  for(int i = 0; i < n; i++){
    cin>>aff[i];
    total += abs(aff[i]);
  }
  
  vector<vector<int>> f(n, vector<int>(n, 0));
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      cin>>f[i][j];
      if(i > j) total += f[i][j];
    }
  }
  
  // Assume Simba is in Team A
  graph G(n);
  edge_adder adder(G);
  long cost1 = 0;
  if(aff[k] < 0) cost1 += -aff[k];
  if(aff[m] > 0) cost1 += aff[m];
  
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(f[i][j])
        adder.add_edge(i, j, f[i][j]);
    }
  }
  
  for(int i = 0; i < n; i++){
    if(i == k || i == m) continue;
    if(aff[i] > 0) adder.add_edge(k, i, aff[i]);
    if(aff[i] < 0) adder.add_edge(i, m, -aff[i]);
  }
  long flow = boost::push_relabel_max_flow(G, k, m);
  cost1 += flow;
  
  // Assume Simba is in Team B
  graph G2(n);
  edge_adder adder2(G2);
  long cost2 = 0;
  if(aff[k] > 0) cost2 += aff[k];
  if(aff[m] < 0) cost2 += -aff[m];
  
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(f[i][j])
        adder2.add_edge(i, j, f[i][j]);
    }
  }
  
  for(int i = 0; i < n; i++){
    if(i == k || i == m) continue;
    if(aff[i] < 0) adder2.add_edge(k, i, -aff[i]);
    if(aff[i] > 0) adder2.add_edge(i, m, aff[i]);
  }
  flow = boost::push_relabel_max_flow(G2, k, m);
  cost2 += flow;
  
  long min_dissatisfaction = min(cost1, cost2);
  
  if(b == 1){
    cout << min_dissatisfaction << "\n";
  } else {
    cout << total - 2 * min_dissatisfaction << "\n";
  }
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}