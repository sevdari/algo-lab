#include<bits/stdc++.h>

using namespace std;

int n, m, k;
vector<int> bright;
vector<int> ans;

multiset<int> ms;
vector<int> path;

void dfs(int root, const vector<vector<int>>& tree){
  
  path.push_back(root);
  ms.insert(bright[root]);
  if(path.size() > m) ms.erase(ms.find(bright[path[path.size() - m - 1]]));
  
  if(path.size() >= m && *ms.rbegin() - *ms.begin() <= k)
    ans.push_back(path[path.size() - m]);
  
  for(auto child: tree[root])
    dfs(child, tree);
  
  if(path.size() > m) ms.insert(bright[path[path.size() - m - 1]]);
  ms.erase(ms.find(bright[path[path.size()-1]]));
  path.pop_back();  
}

void testcase(){
  ans.clear(); bright.clear(); ms.clear(); path.clear();
  cin>>n>>m>>k;
  int h;
  for(int i = 0; i < n; i++){
    cin>>h; bright.push_back(h);
  }
  
  vector<vector<int>> tree(n);
  int u, v;
  for(int i = 0; i < n - 1; i++){
    cin>>u>>v;
    tree[u].push_back(v);
  }
  
  dfs(0, tree);
  sort(ans.begin(), ans.end());
  
  if(ans.size()==0){
    cout << "Abort mission\n"; return;
  }
  
  for(int i = 0; i < ans.size(); i++){
    if(i > 0 && ans[i]==ans[i-1]) continue;
    cout << ans[i] << " ";
  }
  cout << endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}