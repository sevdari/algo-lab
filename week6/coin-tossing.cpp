#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
using namespace std;

// BGL graph type
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    boost::no_property,
    boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
    boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::edge_descriptor edge_desc;

struct edge_adder {
    graph &G;
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int u, int v, long c) {
        auto cap = boost::get(boost::edge_capacity, G);
        auto rev = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(u, v, G).first;
        const auto r = boost::add_edge(v, u, G).first;
        cap[e] = c;
        cap[r] = 0;
        rev[e] = r;
        rev[r] = e;
    }
};

void testcase() {
  
  long players, rounds; cin>>players>>rounds;
  vector<long> points(players, 0);
  vector<long> finish(players, 0);
  vector<pair<long, long>> games;
  long points_so_far = 0, total_points = 0;
  for(long i = 0; i < rounds; i++){
    long a, b, c; cin>>a>>b>>c;
    if(c==1){
      points[a]++;
      points_so_far++;
    } else if(c==2){
      points[b]++;
      points_so_far++;
    } else {
      games.push_back({a, b});
    }
  }
  
  long games_remaining = games.size();
  graph G(games_remaining + players + 2);
  edge_adder adder(G);
  long s = games_remaining + players, t = games_remaining + players + 1;
  
  bool valid = true;
  for(long i = 0; i < players; i++){
    long score; cin>>score;
    finish[i] = score;
    if(score < points[i]) valid = false;
    total_points += score;
    adder.add_edge(games_remaining + i, t, score - points[i]);
  }
  
  if(!valid || total_points - points_so_far != games.size()){
    cout << "no\n";
    return;
  }

  for(long i = 0; i < games_remaining; i++){
    adder.add_edge(s, i, 1);
    auto [a, b] = games[i];
    adder.add_edge(i, games_remaining + a, 1);
    adder.add_edge(i, games_remaining + b, 1);
  }
  
  long flow = boost::push_relabel_max_flow(G, s, t);
  cout << (flow == total_points - points_so_far ? "yes" : "no") << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) testcase();
}
