#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int n, m;
IT a, b, c;

struct Potion {
    int i, j, d;
    IT e;
};

bool solve_lp(
    int k,
    const vector<Potion>& potions
  ){
  Program lp (CGAL::SMALLER, true, 0, false, 0); 

  for(int i = 0, j = 0; i < m; i++){
    Potion potion = potions[i];
    if(potion.i > k || potion.j > k) continue;
    lp.set_a(j, potion.i, 1);
    lp.set_a(j, potion.j, -potion.e);
    lp.set_c(j, potion.d);
    j++;
  }
  lp.set_b(0, -b);
  lp.set_b(1, a);
  
  auto s = CGAL::solve_linear_program(lp, ET());
  return !s.is_infeasible() && s.objective_value_numerator() / s.objective_value_denominator() <= c;
}

void testcase() {
  cin>>n>>m;
  cin>>a>>b>>c;
  vector<Potion> potions(m);
  
  long from, to, e, ee;
  for(int i = 0; i < m; i++){
    cin>>from>>to;
    from--; to--;
    potions[i].i = from; potions[i].j = to;
    cin>>potions[i].d;
    cin >> e >> ee;
    potions[i].e = IT(e, ee);
  }
  
  
  if(!solve_lp(n-1, potions)){
    cout << "Busted!\n"; return;
  }
  
  int left = -1, right = n - 1;
  while(right - left > 1){
    int mid = left + (right - left) / 2;
    if(solve_lp(mid, potions))
      right = mid;
    else
      left = mid;
  }
  
  cout << right + 1 << " " << "7777777" << endl;

}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
