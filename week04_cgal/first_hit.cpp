#include<bits/stdc++.h>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;


#include <CGAL/Ray_2.h>
#include <CGAL/Segment_2.h>

typedef K::Point_2 P;
typedef K::Segment_2 S;


double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(int n){
  long x, y, a, b; cin>>x>>y>>a>>b;
  K::Point_2 start(x, y);
  K::Ray_2 r(start, K::Point_2(a, b));
  K::Segment_2 rs;
  
  vector<K::Segment_2> obs;
  for(int i = 0; i < n; i++){
    cin>>x>>y>>a>>b;
    obs.push_back(K::Segment_2(K::Point_2(x, y), K::Point_2(a, b)));
  }
  random_shuffle(obs.begin(), obs.end());
  
  // clip the ray into a segment
  int j = 0;
  for (; j < n; ++j){
    if (CGAL::do_intersect(r, obs[j])){
      auto o = CGAL::intersection(r,obs[j]);
      const P* op = boost::get<P>(&*o);
      const S* os = boost::get<S>(&*o);
      if (op){
        rs = K::Segment_2(start, *op); break;
      }else if (os){
        if(CGAL::squared_distance(start, os->source()) <= 
          CGAL::squared_distance(start, os->target())  
        )
          rs =  K::Segment_2(start, os->source());
        else
          rs =  K::Segment_2(start, os->target());
      }
      break;
    }
  }
  
  // if unclipped return
  if(j==n){
    cout << "no\n"; return;
  }
  
  // used clipped ray for further checks
  for (; j < n; ++j){
    if (CGAL::do_intersect(rs, obs[j])){
      auto o = CGAL::intersection(rs, obs[j]);
      const P* op = boost::get<P>(&*o);
      const S* os = boost::get<S>(&*o);
      if (op && CGAL::squared_distance(start, *op) <= rs.squared_length()){
        rs = K::Segment_2(start, *op);
      }else{
        if(CGAL::squared_distance(start, os->source()) <= rs.squared_length())
          rs =  K::Segment_2(start, os->source());
        else if (CGAL::squared_distance(start, os->target()) <= rs.squared_length())
          rs =  K::Segment_2(start, os->target());
      }
    }
  }
  
  cout << floor_to_double(rs.target().x()) << " " 
       << floor_to_double(rs.target().y()) << endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t; cin>>t;
  while (t > 0) {
    testcase(t);
    cin>>t;
  }
  return 0;
}