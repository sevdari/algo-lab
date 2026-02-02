#include<bits/stdc++.h>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void testcase(){
  int n, start; cin>>n>>start;
  start--;
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;
  vector<pair<int, pair<int, int>>> edges;
  
  // build graph and get all edges
  for(int i = 0; i < n - 1; i++){
    for(int j = i + 1; j < n; j++){
      int w; cin>>w;
      e = boost::add_edge(i, j, G).first; weights[e]=w;
      edges.push_back({w, {i, j}});
    }
  }
  
  // sort edges by weight
  sort(edges.begin(), edges.end());
  
  // run prim to get first MST
  vector< boost::graph_traits< weighted_graph >::vertex_descriptor > p(num_vertices(G));
  prim_minimum_spanning_tree(G, &p[0], boost::root_vertex(start));
  
  // have an adjacency map but just with edges in the MST
  int mst_weight = 0;
  vector<vector<int>> neighs(n);
  for(int u = 0; u < n; u++){
    int v = p[u];
    if(v==u) continue;
    mst_weight += weights[boost::edge(u, v, G).first];
    neighs[v].push_back(u); neighs[u].push_back(v);
  }
  
  // try removing each edge in the MST and find alternative
  vector<int> comp(n);
  int best = INT_MAX;
  for(int u = 0; u < n; u++){
    int v = p[u]; if(v==u) continue;
    int curr_weight = mst_weight - weights[boost::edge(u, v, G).first];
    for(int i = 0; i < n; i++) comp[i] = -1;
    
    // once we remove an edge we have two connected components. Identify them.
    
    // bfs from u
    queue<int> q; q.push(u);
    comp[u] = 0;
    while(q.size()){
      auto curr = q.front(); q.pop();
      for(auto neigh: neighs[curr]){
        if(comp[neigh]==-1 && neigh!=v){
          q.push(neigh); comp[neigh] = comp[u];
        }
      }
    }
    
    // bfs from v
    comp[v] = 1; q.push(v);
    while(q.size()){
      auto curr = q.front(); q.pop();
      for(auto neigh: neighs[curr]){
        if(comp[neigh]==-1 && neigh!=u){
          q.push(neigh); comp[neigh] = comp[v];
        }
      }
    }
    
    // find edge with minimum weight that connects the two components
    int min_add;
    for(auto edge: edges){
      int w = edge.first, i = edge.second.first, j = edge.second.second;
      if(comp[i]==comp[j]) continue;
      if(i == u && j == v) continue;
      if(i == v && j == u) continue;
      best = min(best, curr_weight + w); break;
    }
  }
  
  cout << best << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}