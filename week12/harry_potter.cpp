///1
#include<bits/stdc++.h>

using namespace std;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef  CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Edge{
  int from;
  int to;
  CGAL::Gmpq sus;
  CGAL::Gmpq eff;
};

double ceil_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

int n, m, a, b, c;

bool solves_lp(const vector<Edge>& edges, int k){
  Program lp (CGAL::LARGER, true, 0, false, 0);
  int var_count = 0;
  for(const auto& edge: edges){
    if(edge.to == 1 || edge.from == 0) continue;
    if(edge.to > k || edge.from > k) continue;
    lp.set_a(var_count, edge.from, -1);
    lp.set_a(var_count, edge.to, edge.eff);
    lp.set_a(var_count, n, -edge.sus);
    var_count++;
  }
  lp.set_b(0, b); lp.set_b(1, -a); lp.set_b(n, -c);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  return !s.is_infeasible();
}

long optimal_solution(const vector<Edge>& edges, int k){
  Program lp (CGAL::LARGER, true, 0, false, 0);
  int var_count = 1; int max_sus = 0;
  for(const auto& edge: edges){
    if(edge.to == 1 || edge.from == 0) continue;
    if(edge.to > k || edge.from > k) continue;
    lp.set_a(var_count, edge.from, -1);
    lp.set_a(var_count, edge.to, edge.eff);
    lp.set_a(var_count, n, -edge.sus);
    lp.set_a(var_count, n + var_count, -edge.sus);
    lp.set_a(max_sus, n + var_count, 1);
    var_count++;
  }
  lp.set_b(0, b); lp.set_b(1, -a); lp.set_b(n, -c);
  lp.set_c(max_sus, 1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  return ceil_to_double(s.objective_value());
}

void testcase(){
  cin>>n>>m>>a>>b>>c;
  vector<Edge> edges(m);
  
  int from, to, e, ee;
  for(int i = 0; i < m; i++){
    cin>>from>>to; from--; to--;
    edges[i].from = from; edges[i].to = to;
    cin>>edges[i].sus;
    cin>>e>>ee;
    edges[i].eff = CGAL::Gmpq(e, ee);
  }
  
  if(!solves_lp(edges, n)){
    cout << "Busted!\n"; return;
  }
  
  int left = 1, right = n;
  while(right - left > 1){
    int mid = left + (right - left) / 2;
    if(solves_lp(edges, mid)){
      right = mid;
    } else {
      left = mid;
    }
  }
  
  cout << right + 1 << " " << optimal_solution(edges, right) << endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}