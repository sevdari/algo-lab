///2

// Solves just test case 2, gotta take care of the sign for the others/
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

typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Line{
  int a;
  int b;
  int c;
  int speed;
  int normal;
};

void testcase() {
  int x, y, n; cin>>x>>y>>n;
  vector<Line> lines(n);
  for(int i = 0; i < n; i++){
    cin>>lines[i].a>>lines[i].b>>lines[i].c>>lines[i].speed;
    // TODO: Check if we need exact sqrt
    lines[i].normal = CGAL::to_double(CGAL::sqrt(1.0 * lines[i].a * lines[i].a + 1.0 * lines[i].b * lines[i].b)); 
  }
  
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  const int t = 0;
  const int p_x = 1;
  const int p_y = 2;
  
  lp.set_l(t, 0);
  lp.set_c(t, -1); 
  
  // legions constraint
  for(int i = 0; i < n; i++){
    // t_i * v_i * norm_i
    lp.set_a(t, i,  lines[i].speed * lines[i].normal); 
    // + a_i * p_x + b_i * p_y
    lp.set_a(p_x, i, lines[i].a); lp.set_a(p_y, i, lines[i].b); 
    // <= -c
    lp.set_b(i, -lines[i].c);
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  cout << floor(CGAL::to_double(-s.objective_value().numerator() / 
          s.objective_value().denominator()))
       << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
