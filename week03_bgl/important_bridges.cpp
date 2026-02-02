///1
#include<bits/stdc++.h>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

namespace boost
{
struct edge_component_t
{
    enum
    {
        num = 555
    };
    typedef edge_property_tag kind;
} edge_component;
}

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
          boost::no_property, boost::property< boost::edge_component_t, std::size_t > >          graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

void testcase(){
  int n, m; cin>>n>>m;
  int u, v;
  graph G(n);
  for(int i = 0; i < m; i++){
    cin>>u>>v;
    boost::add_edge(u, v, G);
  }
  boost::property_map< graph, boost::edge_component_t >::type component
        = get(boost::edge_component, G);
   std::size_t num_comps = boost::biconnected_components(G, component);
  
  vector<int> freq(m, 0);
  boost::graph_traits< graph >::edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei)
      freq[component[*ei]] += 1;
  
  vector<pair<int, int>> ans;
  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei){
    if(freq[component[*ei]] == 1){
      if((int)boost::source(*ei, G) < (int)boost::target(*ei, G)){
        ans.push_back({boost::source(*ei, G), boost::target(*ei, G)});
      } else {
        ans.push_back({boost::target(*ei, G), boost::source(*ei, G)});
      }
    }
  }
  sort(ans.begin(), ans.end());
  cout << ans.size() << endl;
  for(auto [u,v]: ans)
    cout << u << " " << v << endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}