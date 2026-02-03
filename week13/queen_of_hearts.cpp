#include<bits/stdc++.h>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

int get_carrots(){
  int n, m, c; cin>>n>>m>>c;
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  edge_desc e;
 
  int u, v;
  for(int i = 0; i < m; i++){
    cin >> u >> v;
   e = boost::add_edge(u, v, G).first; weights[e]=1;
  } 
  return c - dijkstra_dist(G, 0, n-1);
}

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc_f;
typedef traits::edge_descriptor edge_desc_f;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

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

int r, m, d;

void testcase(){
  cin >> r >> m >> d;
  
  vector<int> carrots(r);
  
  for(int i = 0; i < r; i++){
    carrots[i] = get_carrots();
    if(carrots[i] < 0) carrots[i] = 0;
  }
  
  graph G(2 * r);
  edge_adder adder(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  const vertex_desc_f v_source = boost::add_vertex(G);
  const vertex_desc_f v_sink = boost::add_vertex(G);
  
  // model capacities
  for(int i = 0; i < r; i++)
    adder.add_edge(i, r+i, carrots[i]);
  
  int u, v;
  for(int i = 0; i < m; i++){
    cin>>u>>v;
    u--; v--;
    if(u == -1) u = v_source; else u += r;
    if(v == r) v = v_sink;
    adder.add_edge(u, v, 300);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  int gold_coins = 0;
  out_edge_it e, eend;
  for (boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
    if(rc_map[*e] > 0) gold_coins++;
  }
  
  if(d==0)
    cout << flow << " " << "0" << endl;
  else
    cout << flow << " " << gold_coins << endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}