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
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void print_solution(Solution s, int p){
  if (s.is_infeasible()) {
    cout << "no\n";
    return;
  }
  if (s.is_unbounded()) {
      cout << "unbounded\n";
      return;
  }
  
  auto v = s.objective_value();
  if (p == 1) cout << floor(CGAL::to_double(-v)) << "\n";
  else        cout << ceil(CGAL::to_double(v))  << "\n";

}

void testcase_1(){
  int a, b; cin>>a>>b;
  const int X = 1, Y = 2;
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);   // x + y <= 4
  lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b); // 4x + 2y <= ab
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);  // -x + y <= 1
  // objective function
  lp.set_c(X, a);
  lp.set_c(Y, -b);
  
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  print_solution(s, 1);
}

void testcase_2(){
  int a, b; cin>>a>>b;
  const int X = 1, Y = 2, Z = 3;
  Program lp (CGAL::SMALLER, false, 0, true, 0);
  lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1); lp.set_a(Z, 0, 0); lp.set_b(0, 4);
  lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b); 
  lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_a(Z, 2, 0); lp.set_b(2, 1);
  
  // objective function
  lp.set_c(X, a);
  lp.set_c(Y, b);
  lp.set_c(Z, 1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  print_solution(s, 2);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int p; cin >> p;
  while(p != 0){
    if(p==1) testcase_1();
    else testcase_2();
    cin>>p;
  }
}
