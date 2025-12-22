///1
#include<iostream>
#include<algorithm>
#include<vector>

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



void testcase(){
  int boats, sailors, pairs; cin>>boats>>sailors>>pairs;
  
  // construct graph
  graph G(boats+sailors);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  // source to sailors
  for(int i = 0; i < sailors; i++)
    adder.add_edge(v_source, i, 1, 0);
  
  // boats to sink
  for(int i = 0; i < boats; i++)
    adder.add_edge(sailors + i, v_sink, 1, 0);
  
  // add edges
  vector<vector<int>> coeff(sailors, vector<int>(boats, 0));
  for(int i = 0; i < pairs; i++){
    int b, s, c; cin>>b>>s>>c;
    coeff[s][b] = -c;
  }
  
  for(int i = 0; i < sailors; i++){
    for(int j = 0; j < boats; j++){
      adder.add_edge(i, sailors+j, 1, coeff[i][j]);
    }
  }
  
  // get flow
  int flow1 = boost::push_relabel_max_flow(G, v_source, v_sink);
  boost::cycle_canceling(G);
  int cost1 = boost::find_flow_cost(G);
  std::cout << -cost1 << "\n"; 
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  while(t){testcase(); t--;}
}