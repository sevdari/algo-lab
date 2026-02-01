#include<bits/stdc++.h>

using namespace std;

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

struct Line{
  long a;
  long b;
  long c;
  long v;
  long norm;
};

double floor_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(){
  int x, y, n; cin>>x>>y>>n;
  vector<Line> lines(n);
  
  // add lines, ensure correct orientation
  for(int i = 0; i < n; i++){
    cin>>lines[i].a>>lines[i].b>>lines[i].c>>lines[i].v;
    if(lines[i].a * x + lines[i].b * y + lines[i].c < 0){
      lines[i].a *= -1; lines[i].b *= -1; lines[i].c *= -1;
    }
    lines[i].norm = lines[i].a * lines[i].a + lines[i].b * lines[i].b;
    lines[i].norm = sqrt(lines[i].norm);
  }
  
  Program lp (CGAL::LARGER, false, 0, false, 0); 
  const int X = 0; 
  const int Y = 1;
  const int T = 2;
  
  for(int i = 0; i < n; i++){
    auto line = lines[i];
    lp.set_a(X, i,  line.a); lp.set_a(Y, i, line.b);
    lp.set_a(T, i, -line.norm * line.v);
    lp.set_b(i, -line.c);
  }
  lp.set_c(T, -1);
  
  lp.set_l(T, true, 0);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  cout << floor_to_double(-s.objective_value()) << endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}