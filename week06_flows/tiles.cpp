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

int w, h, N;
vector<vector<char>> board;

const vector<pair<int, int>> moves = {
  {-1, 0}, {1, 0}, {0, 1}, {0, -1}
};

bool valid(int i, int j){
  if(i < 0 || i >= h || j < 0 || j >= w) return false;
  if(board[i][j]=='x') return false;
  return true;
}



void testcase() {
  cin>>w>>h;
  N = h * w;
  board.assign(h, vector<char>(w));
  
  for(int i = 0; i < h; i++)
    for(int j = 0; j < w; j++)
      cin>>board[i][j];
  
  graph G(2 * N);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  edge_adder adder(G);
  
  int valid_tiles = 0;
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      if(board[i][j] == 'x') continue;
      valid_tiles++;
      adder.add_edge(v_source, i * w + j, 1);
      adder.add_edge(N + i * w + j, v_sink, 1);
      for(auto [dx, dy]: moves){
        if(valid(i + dx, j + dy))
          adder.add_edge(i * w + j, N + (i + dx) * w + (j + dy), 1);
      }
    }
  }
  
  int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  cout << ((flow == valid_tiles) ? "yes\n" : "no\n");
  
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
