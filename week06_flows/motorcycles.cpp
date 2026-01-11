/*
Solve only the first 3 test cases.
*/
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

struct Biker{
  long y0, x1, y1;
  int ind;
};


bool bigger_angle(const Biker& a, const Biker& b){
  return (__int128)a.x1 * (b.y1 - b.y0) >=
         (__int128)b.x1 * (a.y1 - a.y0);
}

void testcase() {
  // input
  int n; cin>>n;
  vector<Biker> bikers;
  
  for(int i = 0; i < n; i++){
    Biker b;
    cin >> b.y0 >> b.x1 >> b.y1;
    b.ind = i;
    bikers.push_back(b);
  }
  
  // highest to lowest
  sort(bikers.begin(), bikers.end(), [](const Biker& a, const Biker& b){
    return a.y0 > b.y0;
  });
  
  // keep track of max angle and discard any biker who has smaller angle
  Biker max_angle_biker = bikers[0];
  vector<int> res;
  for(int i = 0; i < n; i++){
    if(bigger_angle(bikers[i], max_angle_biker)){
      res.push_back(bikers[i].ind);
      max_angle_biker = bikers[i];
    }
  }
  
  // output
  sort(res.begin(), res.end());
  for(auto ind: res)
    cout << ind << " ";
  cout << endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
