#include<iostream>
#include<vector>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

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

void testcase_1(int a, int b){
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  
  // set the coefficients of A and b
  const int X = 0; 
  const int Y = 1;
  
  // min aX - bY
  lp.set_c(X, a);
  lp.set_c(Y, -b);
  
  lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);  //  x + y  <= 4
  lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);  // 4x + 2y <= ab
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); // -x + y <= 1
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  print_solution(s, 1);
}

void testcase_2(int a, int b){
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, false, 0, true, 0); 
  
  // set the coefficients of A and b
  const int X = 0; 
  const int Y = 1;
  const int Z = 2;
  
  // min aX + bY + z
  lp.set_c(X, a);
  lp.set_c(Y, b);
  lp.set_c(Z, 1);
  
  lp.set_a(X, 0,  -1); lp.set_a(Y, 0, -1); lp.set_b(0, 4);  //  -x - y  <= 4
  lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b);  // -4x - 2y - z <= ab
  lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_b(2, 1); // x - y <= 1
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  print_solution(s, 2);
}


int main(){
  ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  int p, a, b; cin>>p;
  while(p!=0){
    cin >> a >> b;
    if(p==1){
      testcase_1(a, b);
    } else{
      testcase_2(a, b);
    }
    cin>>p;
  }
}