#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

void testcase() {
  int n; cin>>n;
  vector<int> start(n), finish(n);
  for(int i = 0; i < n; i++)
    cin >> start[i] >> finish[i];
  
  sort(start.begin(), start.end());
  sort(finish.begin(), finish.end());
  
  int s = 0, f = 0, count = 0, best = 0;
  while(s < n){
    if(start[s] <= finish[f]){
      s++; count++;
    } else {
      f++; count--;
    }
    best = max(best, count);
  }
  cout << best << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}