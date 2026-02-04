/*
Very rough draft
*/

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

double floor_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

struct Warehouse{
  int x;
  int y;
  int s;
  int a;
};

struct Stadium{
  int x;
  int y;
  int d;
  int u;
};

struct Contour{
  int x;
  int y;
  int r;
};

int n, m, c;

int var(int i, int j){
  return i * m + j;
}

void testcase(){
  cin>>n>>m>>c;
  vector<Warehouse> warehouse(n);
  vector<Stadium> stadium(m);
  vector<Contour> contour(c);

  for(int i = 0; i < n; i++)
    cin>>warehouse[i].x>>warehouse[i].y>>warehouse[i].s>>warehouse[i].a;
  
  for(int i = 0; i < m; i++)
    cin>>stadium[i].x>>stadium[i].y>>stadium[i].d>>stadium[i].u;
  
  vector<vector<int>> revenues(n, vector<int>(m, 0));
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      cin>>revenues[i][j];
    }
  }

  for(int i = 0; i < c; i++)
    cin>>contour[i].x>>contour[i].y>>contour[i].r;
  
  Program lp (CGAL::SMALLER, true, 0, false, 0); 

  // stadium demand + alcohol upper limit
  for(int j = 0; j < m; j++){
    for(int i = 0; i < n; i++){
      lp.set_a(var(i, j), j,  1);                      // smaller than demand
      lp.set_a(var(i, j), m + j,  -1);                 // greater than demand
      lp.set_a(var(i, j), 2 * m + j, warehouse[i].a);  // alcohol upper limit 
    }
    lp.set_b(j,         stadium[j].d); 
    lp.set_b(m + j,     -stadium[j].d); 
    lp.set_b(2 * m + j, stadium[j].u * 100); 
  }

  // supply of warehouses
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      lp.set_a(var(i, j), 3 * m + i,  1);   
    }
    lp.set_b(3 * m + i, warehouse[i].s);
  }

  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      lp.set_c(var(i, j), revenues[i][j]);   
    }
  }
  

  Solution s = CGAL::solve_linear_program(lp, ET());
  if(!s.is_infeasible()){
    cout << floor_to_double(-s.objective_value()) << endl;
  } else {
    cout << "RIOT!\n";
  }

}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}