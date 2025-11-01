#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list< 
  boost::vecS, 
  boost::vecS, 
  boost::undirectedS 
  > graph;

int intersection_size(const unordered_set<string>& a, const unordered_set<string>& b) {
    int count = 0;
    for (const auto& x : a)
      if (b.count(x))
        ++count;
    return count;
}


void testcase(){
  int n, c, f; cin>>n>>c>>f;
  graph g(n);
  vector<unordered_set<string>> characteristics(n);
  for(int i = 0; i < n; i++){
    for(int j = 0; j < c; j++){
      string chara; cin>>chara;
      characteristics[i].insert(chara);
    }
  }
  
  for(int i = 0; i < n; i++){
    for(int j = i+1; j < n; j++){
      if(intersection_size(characteristics[i], characteristics[j]) > f){
        add_edge(i, j, g);
      }
    }
  }
  
  
  vector< boost::graph_traits< graph >::vertex_descriptor > mate(n);

  boost::checked_edmonds_maximum_cardinality_matching(g, &mate[0]);
  int matching_size = boost::matching_size(g, &mate[0]);
  
  if (matching_size * 2 == n) {
    cout << "not optimal\n";
  } else {
    cout << "optimal\n";
  }

}

int main(){
  ios_base::sync_with_stdio(false);
  int tests; cin>>tests;
  while(tests--){
    testcase();
  }
}