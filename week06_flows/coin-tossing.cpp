///1
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
  int n, m; cin>>n>>m;
  vector<int> points(n, 0);
  vector<pair<int, int>> matches;
  
  int a, b, c;
  for(int i = 0; i < m; i++){
    cin>>a>>b>>c;
    if(c == 0)
      matches.push_back({a, b});
    else
      if(c==1) points[a]++; else points[b]++;
  }
  
  int s;
  bool flag = false;
  int need = 0;
  for(int i = 0; i < n; i++){
    cin>>s;
    if(s < points[i])
      flag = true;
    points[i] = s - points[i];
    need += points[i];
  }
  
  if(flag || need!=matches.size()){ cout << "no\n"; return;}
  
  graph G(matches.size() + n);
  edge_adder adder(G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for(size_t i = 0; i < matches.size(); i++){
    auto [a, b] = matches[i];
    adder.add_edge(v_source, i, 1);
    adder.add_edge(i, matches.size() + a, 1);
    adder.add_edge(i, matches.size() + b, 1);
  }
  
  for(size_t i = 0; i < n; i++)
    adder.add_edge(matches.size() + i, v_sink, points[i]);
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  cout << (flow == need ? "yes\n": "no\n");
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
