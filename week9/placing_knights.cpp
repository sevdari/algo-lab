#include<iostream>
#include<vector>
#include <algorithm>
#include <queue>

using namespace std;

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


vector<pair<int,int>> moves = {
  {-1, -2}, 
  {-1, 2}, 
  {1, -2}, 
  {1, 2}, 
  {-2, -1}, 
  {-2, 1}, 
  {2, -1},
  {2, 1}
};


bool valid(int i, int j, int n){
  return i >= 0 && j >= 0 && i < n && j < n;
}


void testcase(){
  int n; cin>>n;
  int count=0;
  
  // construct graph
  graph G(n*n);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  // get board
  vector<vector<bool>> board(n, vector<bool>(n, false));
  for(int i=0; i<n; i++){
    for(int j=0;j<n;j++){
      int temp; cin>>temp;
      if(temp) {
        board[i][j] = true;
        count += 1;
      }
      if((i+j)%2==0){
        adder.add_edge(v_source, i*n+j, 1);
      } else {
        adder.add_edge(i*n+j, v_sink, 1);
      }
    }
  }
  
  // add edges
  for(int i=0; i<n; i++){
    for(int j=0;j<n;j++){
      if(!board[i][j]) continue;
      if((i + j)%2==0){
        for (auto [dx, dy] : moves) {
          if(valid(i + dx, j + dy, n) && board[i+dx][j+dy]){
          adder.add_edge(i*n+j, (i+dx)*n+(j+dy), 1);
          }
        }
      }
    }
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  cout << count - flow << endl;
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  while(t>0){
    testcase();
    t--;
  }
}