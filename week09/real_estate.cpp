///1
#include <iostream>
#include<algorithm>
#include<vector>
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


using namespace std;

void testcase(){
  // read input
  int n, m, s; cin>>n>>m>>s;
  vector<int> limits(s);
  vector<int> belongs_to(m);
  for(int i=0; i<s; i++){
    cin>>limits[i];
  }
  for(int i=0; i<m; i++){
    cin>>belongs_to[i];
  }
  
  // construct graph
  graph G(n+m+s);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  for(int i=0; i<n; i++){
    adder.add_edge(v_source, i, 1, 0); // each person gets most one bid
  }
  
  for(int i=0; i<n;i++){
    for(int j=0; j<m; j++){
      int bid; cin>>bid;
      adder.add_edge(i, n + j, 1, -bid); // min cost so negative bid
    }
  }
  
  for(int j=0; j<m; j++){
    adder.add_edge(n+j, n+m+belongs_to[j]-1, 1, 0); // connect lots to state
  }
  
  for(int j=0; j<s; j++){
    adder.add_edge(n+m+j, v_sink, limits[j], 0); // connect states to sink
  }
  
  int flow1 = boost::push_relabel_max_flow(G, v_source, v_sink);
  boost::cycle_canceling(G);
  int cost1 = boost::find_flow_cost(G);
  cout << flow1 << " " << -cost1 << "\n"; 
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  while(t){
    t--;
    testcase();
  }
}