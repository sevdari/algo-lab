///2

// Only solves test case 2.

#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>


// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void testcase() {
  int n, start; cin>>n>>start;
  start -= 1;
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  edge_desc e; int w;
  vector<vector<int>> board(n, vector<int>(n, 0));
  for(int i = 0; i < n; i++){
    for(int j = i + 1; j < n; j++){
      cin >> w;
      e = boost::add_edge(i, j, G).first; weights[e]=w;
      board[i][j] = w; board[j][i] = w;
    }
  }
  
  vector<vertex_desc> p(n);
  auto index = get(boost::vertex_index, G);
  
  boost::prim_minimum_spanning_tree(
      G,
      boost::make_iterator_property_map(p.begin(), index),
      boost::root_vertex(start)
  );
  
  // for (std::size_t i = 0; i != p.size(); ++i)
  //   if (p[i] != i)
  //       std::cout << "parent[" << i << "] = " << p[i] << std::endl;
  //   else
  //       std::cout << "parent[" << i << "] = no parent" << std::endl;
  
  int weight = 0;
  vector<int> deg(n, 0);
  for (int i = 0; i < n; i++) {
      if (p[i] != i) {
          weight += board[i][p[i]];
          deg[i]++;
          deg[p[i]]++;
      }
  }
  
  int star = -1;
  for (int i = 0; i < n; i++) {
      if (deg[i] == n-1) star = i;
  }

  int min_alternative = std::numeric_limits<int>::max();
  
  for(int i = 0; i < n; i++){
    for(int j = i+1; j < n; j++){
      if(i==star || j==star) continue;
      int temp = weight - max(board[star][i], board[star][j]);
      min_alternative = min(min_alternative, temp + board[i][j]);
    }
  }
  
  cout << min_alternative << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
