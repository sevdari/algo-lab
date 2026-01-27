///2
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

double ceil_to_double(const CGAL::Quotient<IT>& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}


void solve_lp(
    const vector<vector<int>>& p,
    const vector<vector<IT>>& sus,
    const vector<vector<IT>>& eff
  ){
  Program lp (CGAL::SMALLER, true, 0, false, 0); 

  // source contraint \sum p_1j <= a
  for(int j = 0; j < n; j++)
    lp.set_a(p[1][j], 0,  1);
  lp.set_b(0, a);
  
  // sink contraint \sum p_i0 >= b
  for(int i = 1; i < n; i++)
    lp.set_a(p[i][0], 1, -eff[i][0]);
  lp.set_b(1, -b);
  
  // doubt constraint \sum sus_ij p_ij <= c
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      lp.set_a(p[i][j], 2, sus[i][j]);
    }
  }
  lp.set_b(2, c);
  
  // flow constraints \sum e_ij p_ij - \sum p_ji >= 0
  for(int i = 2; i < n; i++){
    for(int j = 1; j < n; j++)
      lp.set_a(p[j][i], i+1, -eff[j][i]);  // into i
    
    for(int j = 0; j < n; j++)
      lp.set_a(p[i][j], i+1, 1);          // out of i
    
    
    lp.set_b(i + 1, 0);
  }
  
  int s_var = n * n;  
  int constraint_idx = n + 1;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(sus[i][j] == 0) continue;
      lp.set_a(p[i][j], constraint_idx, sus[i][j]);
      lp.set_a(s_var, constraint_idx, -1);
      lp.set_b(constraint_idx, 0);
      constraint_idx++;
    }
  }
  
  lp.set_c(s_var, 1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
    
  if(s.is_infeasible())
    cout << "Busted!\n";
  else
    cout << n << " " << ceil_to_double(s.objective_value()) << endl;
}

void testcase() {
  cin>>n>>m;
  cin>>a>>b>>c;
  vector<vector<int>> p(n, vector<int>(n, 0));
  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
      p[i][j] = i * n + j;
  
  vector<vector<IT>> sus(n, vector<IT>(n, IT(0)));
  vector<vector<IT>> eff(n, vector<IT>(n, IT(0)));
  long e, ee;
  for(int i = 0; i < m; i++){
    int from, to; cin>>from>>to;
    from--; to--;
    cin>>sus[from][to];
    cin >> e >> ee;
    eff[from][to] = CGAL::Gmpq(e, ee);
    // cout << "Edge " << from << "->" << to << " eff=" << eff[from][to] << " sus=" << sus[from][to] << "\n";
  }
  
  solve_lp(p, sus, eff);
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
