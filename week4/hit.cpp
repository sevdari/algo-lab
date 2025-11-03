#include <iostream>
using namespace std;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

void testcase(int n){
  long x, y, a, b; cin>>x>>y>>a>>b;
  R ray = R(P(x, y), P(a, b));
  
  for(int i = 0; i < n; i++){
    long r, s, t, u; cin>>r>>s>>t>>u;
    S current = S(P(r, s), P(t, u));
    
    if(CGAL::do_intersect(ray, current)){
      for (; i < n - 1; i++)
        cin >> r >> s >> t >> u;
      cout << "yes" << endl;
      return;
    }
  }
  cout << "no" << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int n; cin>>n;
  while(n!=0){
    testcase(n);
    cin>>n;
  }
}