///3
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

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpq or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

vector<int> f, c, k, l, m;
vector<long> a(25), p(25);
int n; 


bool run_lp(long a_sum, long p_sum){
  Program lp (CGAL::SMALLER, true, 0, true, 1); 
  const int h_1 = 0, h_2 = 1, h_3 = 2;
  
  for(int i = 0; i < n; i++){
    // h_1*k_i + h_2 * l_i + h_3 * m_i >= f_i - c_i 
    lp.set_a(h_1, i, -k[i] - a_sum*a_sum);  
    lp.set_a(h_2, i, -l[i] - p_sum*p_sum);
    lp.set_a(h_3, i, -m[i] - a_sum*p_sum);
    lp.set_b(i, c[i] - f[i]); 
    
  }
  
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  return !s.is_infeasible();
}

void testcase() {
  cin>>n;
  f.resize(n); c.resize(n); k.resize(n); l.resize(n); m.resize(n);
  a[0] = 0; p[0] = 0;
  for(int i = 0; i < n; i++)
    cin>>f[i]>>c[i]>>k[i]>>l[i]>>m[i];
  
  for(int i = 1; i < 25; i++){
    cin>>a[i];
    a[i] += a[i-1];
  }
  
  for(int i = 1; i < 25; i++){
    cin>>p[i];
    p[i] += p[i-1];
  }
  
  int best = 49;
  for(int a_ind = 0; a_ind < 25; a_ind ++){
    int left = 0, right = min(24, best - a_ind);
    while(left < right){
      int mid = left + (right - left) / 2;
      if(run_lp(a[a_ind], p[mid])){
        best = min(best, a_ind + mid);
        right = mid;
      } else {
        left = mid + 1;
      }
    }
  }
  
  if(best == 49)
    cout << "Impossible!\n";
  else
    cout << best << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
