#include<iostream>
#include<vector>
using namespace std;

int main(){
  std::ios_base::sync_with_stdio(false);
  int tests; cin>>tests;
  while(tests--){
    int n; cin>>n;
    vector<int> heights(n);
    vector<int> max_fall(n, 0);
    for(int i = 0; i < n; i++){
      cin>>heights[i];
    }
    int current_max = heights[0] - 1;
    int current = 0;
    while(current < n && current <= current_max){
      if(current + heights[current] - 1 > current_max){
        current_max = current + heights[current] - 1;
      }
      current += 1;
    }
    cout << current << endl;
       
  }
}