// ALGOLAB BGL Tutorial 3
// Flow example demonstrating
// - breadth first search (BFS) on the residual graph

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// graph Type with nested interior edge properties for flow algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >	graph;
// interior property maps
typedef	boost::graph_traits<graph>::edge_descriptor			edge_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;

// custom edge adder class: holds references to the graph, capacity
//   map and reverse edge map
class edge_adder {
  graph &G;
  
public:
  explicit edge_adder(graph &G) : G(G) {}
  
  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int main() {
  // build graph
  const int N = 6;
  graph G(N);
  edge_adder adder(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  int src = 0;
  int sink = 5;

  // add edges
  adder.add_edge(src, 1, 5);
  adder.add_edge(1, 2, 3);
  adder.add_edge(1, 3, 2);
  adder.add_edge(2, 3, 1);
  adder.add_edge(2, 4, 2);
  adder.add_edge(3, 4, 2);
  adder.add_edge(4, sink, 4);

  // find a min cut via maxflow
  int flow = boost::push_relabel_max_flow(G, src, sink);
  std::cout << "maximum flow = minimum cut = " << flow << "\n";

  // BFS to find vertex set S
  std::vector<int> vis(N, false); // visited flags
  std::queue<int> Q; // BFS queue
  vis[src] = true; // mark the source as visited
  Q.push(src);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      // follow edges with spare capacity only
      if (rc_map[*ebeg] == 0 || vis[v]) continue;
      vis[v] = true;
      Q.push(v);
    }
  }

  // output S
  for (int i = 0; i < N; ++i) 
    if (vis[i]) std::cout << i << " ";
  std::cout << "\n";
}
