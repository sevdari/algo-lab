/*
Very rough draft.
*/


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
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

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

struct Request{
  int s;
  int t;
  int d;
  int a;
  int p;
};

vector<vector<int>> memo;
vector<Request> res;

void testcase(){
  int N, S; cin>>N>>S;
  vector<int> dist(S);
  res = vector<Request>(N);

  memo = vector<vector<int>>(10001, vector<int>(2, -1));

  for(int i = 0; i < S; i++)
    cin>>dist[i];
  
  for(int i = 0; i < N; i++)
    cin>>res[i].s>>res[i].t>>res[i].d>>res[i].a>>res[i].p;
  
  int V = 334;
  graph G(V * 2);
  edge_adder adder(G);

  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  adder.add_edge(v_source, 0,   dist[0], 0);
  adder.add_edge(v_source, V, dist[1], 0);

  adder.add_edge(V-1, v_sink, dist[0] + dist[1], 0);
  adder.add_edge(2*V-1, v_sink, dist[0] + dist[1], 0);

  for(int i = 0; i < V-1; i++){
    adder.add_edge(i, i+1, 1, 100);
    adder.add_edge(V + i, V+i+1, 1, 100);
  }

  
    for (auto& r : res) {
      int dep_time = r.d / 30;
      int arr_time = r.a / 30;
      int from = (r.s - 1) * V + dep_time; 
      int to = (r.t - 1) * V + arr_time;
      
      int skipped = arr_time - dep_time;
      adder.add_edge(from, to, 1, skipped * 100 - r.p);
    }

  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost2 = boost::find_flow_cost(G);
  long baseline = (dist[0] + dist[1]) * (V - 1) * 100;

  cout << baseline - cost2 << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}