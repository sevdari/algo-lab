///1
#include<bits/stdc++.h>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Asteroid{
  K::Point_2 pos;
  int d;
};

void testcase(){
  int a, s, b; cin>>a>>s>>b;
  int e; cin>>e;
  vector<Asteroid> asteroids(a);
  vector<K::Point_2> shots(s), bounty(b);
  
  int x, y;
  for(int i = 0; i < a; i++){
    cin>>x>>y;
    asteroids[i].pos = K::Point_2(x, y);
    cin>>asteroids[i].d;
  }
  
  for(int i = 0; i < s; i++){
    cin>>x>>y;
    shots[i] = K::Point_2(x, y);
  }
  
  for(int i = 0; i < b; i++){
    cin>>x>>y;
    bounty[i] = K::Point_2(x, y);
  }
  
  vector<vector<long>> dist(a, vector<long>(s));
  for(int i = 0; i < a; i++){
    for(int j = 0; j < s; j++){
      dist[i][j] = CGAL::squared_distance(asteroids[i].pos, shots[j]);
      if(dist[i][j]==0) dist[i][j] = 1;
    }
  }
  
  Program lp (CGAL::LARGER, true, 0, false, 0);
  for(int i = 0; i < a; i++){
    for(int j = 0; j < s; j++){
      lp.set_a(j, i,  CGAL::Gmpq(1L, dist[i][j]));
    }
    lp.set_b(i, asteroids[i].d);
  }
  
  for(int j = 0; j < s; j++){
    lp.set_a(j, a , -1);
  }
  lp.set_b(a, -e);
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  if(sol.is_infeasible()) cout << "n\n"; else cout << "y\n";
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}