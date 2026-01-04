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
  long even = 0, odd = 0, total = 0;
  int current;

  for(int i = 0; i < n; i++){
    cin>>current;
    if(current==0){
      even = even + 1;
    } else {
      swap(odd, even);
      odd += 1;
    }
    total += even;
  }
  
  cout << total << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
