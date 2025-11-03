#include <iostream>
#include <vector>
using namespace std;

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef K::Point_2 P;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

double ceil_to_double(const K::FT& x) {
  double a = ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}


void testcase(int n){
  std::vector<K::Point_2> houses;
  
  for(int i = 0; i < n; i++){
    double x, y; cin>>x>>y;
    houses.push_back(P(x, y));
  }
  Min_circle mc(houses.begin(), houses.end(), true);
  Traits::Circle c = mc.circle();
  cout << ceil_to_double(CGAL::sqrt(c.squared_radius())) << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  int n; cin>>n;
  while(n!=0){
    testcase(n);
    cin>>n;
  }
}