#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

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

void testcase(int n, int m) {
  // read input
  vector<int> min_daily(n), max_daily(n);
  vector<int> prices(m);
  vector<vector<int>> product_nutrients(m, vector<int>(n));
  for(int i = 0; i < n; i++){
    cin>>min_daily[i]>>max_daily[i];
  }
  
  for(int i = 0; i < m; i++){
    cin>>prices[i];
    for(int j = 0; j < n; j++){
      cin>>product_nutrients[i][j];
    }
  }
  
  // solution
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      lp.set_a(j, i, -product_nutrients[j][i]); // more than min
      lp.set_a(j, n + i, product_nutrients[j][i]); // less than max
    }
    lp.set_b(i, -min_daily[i]);
    lp.set_b(n+i, max_daily[i]);
  }
  
  // objective
  for(int j = 0; j < m; j++){
    lp.set_c(j, prices[j]);
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if(s.is_optimal())
    cout << floor(CGAL::to_double(s.objective_value())) << endl;
  else
    cout << "No such diet.\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);

  int n, m;
  while(cin>>n>>m && n!=0){
    testcase(n, m);
  }
}
