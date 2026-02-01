///4
#include<bits/stdc++.h>

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

struct Stall{
  long f;
  long c;
  long k;
  long l;
  long m;
};

int n;

bool solve_lp(long a, long p, const vector<Stall>& stalls){
  Program lp (CGAL::LARGER, true, 0, true, 1);
  const int h_1 = 0; const int h_2 = 1; const int h_3 = 2;
  for(int i = 0; i < n; i++){
    const auto& stall = stalls[i];
    lp.set_a(h_1, i,  stall.k + a * a);
    lp.set_a(h_2, i,  stall.l + p * p);
    lp.set_a(h_3, i,  stall.m + a * p);
    lp.set_b(i, stall.f - stall.c);
  }
  Solution s = CGAL::solve_linear_program(lp, ET());
  return !s.is_infeasible();
}


void testcase(){
  cin>>n;
  vector<Stall> stalls(n);
  vector<long> a(25), p(25);
  a[0] = 0; p[0] = 0;
  
  for(int i = 0; i < n; i++)
    cin>>stalls[i].f>>stalls[i].c>>stalls[i].k>>stalls[i].l>>stalls[i].m;
  
  for(int i = 1; i < 25; i++){
    cin>>a[i];
    a[i] += a[i-1];
  }
  
  for(int i = 1; i < 25; i++){
    cin>>p[i];
    p[i] += p[i-1];
  }
  
  // find best combination
  int best = 49, i = 0, j = 24;
  while(i<=24 && j>=0){
    if(solve_lp(a[i], p[j], stalls)){
      best = min(best, i + j);
      j--;
    } else {
      i++;
    }
  }
  
  if(best!=49)
    cout << best << endl;
  else
    cout <<"Impossible!\n";
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}