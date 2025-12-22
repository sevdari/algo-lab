///2
#include<iostream>
#include<algorithm>
#include<vector>


using namespace std;

vector<int> post;

void dfs(int u, vector<vector<int>>& neigh){
    for(int v : neigh[u])
        dfs(v, neigh);
    post.push_back(u);   // postorder
}


void testcase(){
  post.clear();
  int n, m, window; cin>>n>>m>>window;
  vector<int> b(n);
  for(int i = 0; i < n; i++)
    cin>>b[i];
  
  vector<vector<int>> neigh(n);
  for(int i = 0; i < n - 1; i++){
    int u, v; cin>>u>>v;
    neigh[u].push_back(v);
  }
  
  vector<vector<vector<pair<int, int>>>> 
    hist(
      n, 
      vector<vector<pair<int, int>>>(m+1)
    );
  
  for(int i = 0; i < n; i++){
    hist[i][1].push_back({b[i], b[i]});
  }
  
  dfs(0, neigh);

  for(int i : post){
    if(!neigh[i].size()) continue;
    for(int curr_neigh : neigh[i]){
    
      for(int k = 1; k < m; k++){
        if(!hist[curr_neigh][k].size()) break;
        
        for(auto [curr_min, curr_max] : hist[curr_neigh][k]){
          if(b[i] > curr_max) curr_max = b[i];
          if(b[i] < curr_min) curr_min = b[i];
          if(curr_max - curr_min <= window) hist[i][k+1].push_back({curr_min, curr_max});
        }
      }
    }
  }
  
  bool flag = true;
  for(int i = 0; i < n; i++){
    if(hist[i][m].size()){ 
      cout << i << " ";
      flag = false;
    }
  }
  if(flag) cout <<"Abort mission";
  cout << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  while(t){testcase(); t--;}
}