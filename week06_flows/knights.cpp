///1
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

int n, m, k, total;

vector<pair<int, int>> moves = {
  {0, 1},
  {1, 0},
  {-1, 0},
  {0, -1}
};

bool valid(int i, int j){
  if(i < 0 || i >= n) return false;
  if(j < 0 || j >= m) return false;
  return true;
}

int in(int i, int j){
  return i * m + j;
}

int out(int i, int j){
  return total + i * m + j;
}

void testcase(){
  cin>>n>>m>>k;
  int C; cin>>C;
  
  vector<vector<int>> board(n, vector<int>(m, 0));
  
  int x, y;
  for(int i = 0; i < k; i++){
    cin>>x>>y;
    board[x][y] = 1;
  }
  
  total = n * m;
  graph G(2 * total);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      // source to knights
      if(board[i][j]) adder.add_edge(v_source, in(i, j), 1);
      
      int count_invalid = 0;
      // intersection to intersection
      for(auto [dx, dy]: moves){
        if(valid(i + dx, j + dy)){
          adder.add_edge(out(i, j), in(i + dx, j + dy), 1);
        } else {
          count_invalid++;
        }
      }
      
      // intersections to sinks
      if(count_invalid > 0) adder.add_edge(out(i, j), v_sink, count_invalid);
    
      // intersections to themselves (i.e modelling capacity)
      adder.add_edge(in(i, j), out(i, j), C);
    }
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  cout << flow << endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}

