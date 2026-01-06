#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;

typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

double ceil_to_double(const K::FT& x) {
  double a = ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(int n) {
  vector<K::Point_2> citizens;
  for(int i = 0; i < n; i++){
    long x, y; cin>>x>>y;
    citizens.push_back(K::Point_2(x, y));
  }
  
  Min_circle mc(citizens.begin(), citizens.end(), true);
  Traits::Circle c = mc.circle();
  K::FT d = CGAL::sqrt(c.squared_radius());
  std::cout << ceil_to_double(d) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);

  int n; cin >> n;
  while(n!=0){
    testcase(n); cin>>n;
  }
}
