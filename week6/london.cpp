#include <iostream>
#include<string>
#include<vector>
#include<algorithm>
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

    c_map[e]     = capacity;
    c_map[rev_e] = 0;
    r_map[e]     = rev_e;
    r_map[rev_e] = e;
  }

  void increment_capacity(int from, int to) {
    auto cap = boost::get(boost::edge_capacity, G);

    edge_desc e;
    bool ok;
    tie(e, ok) = boost::edge(from, to, G);

    if (ok) {
      cap[e] += 1;          
    } else {
      add_edge(from, to, 1); 
    }
  }
};

void testcase(){
  int h, w; 
  cin >> h >> w;

  string note; 
  cin >> note;

  vector<int> need(26, 0);
  for(char c : note) need[c-'A']++;

  vector<int> frontCnt(26, 0);
  vector<int> backCnt(26, 0);
  vector<vector<int>> pairCnt(26, vector<int>(26, 0));

  vector<string> F(h), B(h);
  for(int i=0;i<h;i++) cin >> F[i];
  for(int i=0;i<h;i++){
    cin >> B[i];
    reverse(B[i].begin(), B[i].end());
  }

  for(int i=0;i<h;i++){
    for(int j=0;j<w;j++){
      int f = F[i][j] - 'A';
      int b = B[i][j] - 'A';
      frontCnt[f]++;
      pairCnt[f][b]++;
    }
  }

  // ---- build graph ----
  graph G(26 + 2);
  int s = 26, t = 27;
  edge_adder adder(G);

  // s → letters in front
  for(int c=0;c<26;c++){
    if(frontCnt[c] > 0) 
      adder.add_edge(s, c, frontCnt[c]);
  }

  // letter → letter (pair edges)
  for(int f=0;f<26;f++){
    for(int b=0;b<26;b++){
      if(pairCnt[f][b] > 0)
        adder.add_edge(f, b, pairCnt[f][b]);
    }
  }

  // letters → sink
  for(int c=0;c<26;c++){
    if(need[c] > 0)
      adder.add_edge(c, t, need[c]);
  }

  long flow = boost::push_relabel_max_flow(G, s, t);

  cout << (flow == note.size() ? "Yes\n" : "No\n");
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin>>t;
  while(t--){
    testcase();
  }
}
