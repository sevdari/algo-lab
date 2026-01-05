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
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  return matching_size;
}

int get_common_chars(const vector<string>& a,const vector<string>& b){
  size_t i = 0, j = 0;
  int count = 0;
  while(i < a.size() && j < b.size()){
    if(a[i] == b[j]){
      count ++; i++; j++;
    } else if (a[i] > b[j]){
      j++;
    } else {
      i++;
    }
  }
  return count;
}

void testcase() {
  int n, c, f; cin>>n>>c>>f;
  vector<vector<string>> chars(n, vector<string>(c)); 
  for(int i = 0; i < n; i++){
    for(int j = 0; j < c; j++)
      cin>>chars[i][j];
    sort(chars[i].begin(), chars[i].end());
  }
  graph G(n);

  for(int i = 0; i < n; i++){
    for(int j = i + 1; j < n; j++){
      int common_count = get_common_chars(chars[i], chars[j]);
      if(common_count > f)  boost::add_edge(i, j, G);
    }
  }
  
  int matching_size = maximum_matching(G);
  cout << (matching_size * 2 == n ? "not " : "");
  cout << "optimal" << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
