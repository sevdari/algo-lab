// This is pretty much a copy of the solution provided in the lecture slides.

#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
 boost::no_property, // no vertex properties
 boost::property<boost::edge_name_t, int>> Graph;

typedef boost::property_map<Graph, boost::edge_name_t>::type ComponentMap; 

typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

void testcase() {
  // input
  int V, E; cin>>V>>E;
  Graph G(V);
  for(int i = 0; i < E; i++){
    int u, v; cin>>u>>v;
    boost::add_edge(u, v, G);
  }
  
  // solution
  ComponentMap componentmap = boost::get(boost::edge_name, G);
  int ncc = boost::biconnected_components(G, componentmap);
  std::vector<int> componentsize(ncc);
  EdgeIt ebeg, eend;
  for (tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
    componentsize[componentmap[*ebeg]]++;
  }
  // Solution vector to record bridges
  std::vector<std::pair<int,int> > bridges;
  for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
    if (componentsize[componentmap[*ebeg]] == 1) { // If edge in a
      int u = source(*ebeg, G); // component of size = 1
      int v = target(*ebeg, G);
      bridges.push_back(std::make_pair(std::min(u,v),
      std::max(u,v)));
    }
  }
  std::sort(bridges.begin(), bridges.end());
  // Output
  std::cout << bridges.size() << std::endl;
  for (size_t i = 0; i < bridges.size(); ++i) {
    std::cout << bridges[i].first << " " <<
    bridges[i].second << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
