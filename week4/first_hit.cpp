/*
This solution gets only 90/100 points 
due to not having implemented clipping.
*/
#include <limits>
#include <cmath>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2   R;
typedef K::Segment_2 S;

double floor_to_double(const K::FT& x) {
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(int n){
  long x,y,a,b; cin >> x >> y >> a >> b;
  P start(x,y), dir(a,b);
  R ray(start, dir);

  bool have_hit = false;
  P best;
  K::FT best_dist;

  for (int i=0;i<n;i++){
    long r,s,t,u; cin >> r >> s >> t >> u;
    S seg(P(r,s), P(t,u));

    if (!CGAL::do_intersect(ray, seg)) continue;

    auto o = CGAL::intersection(ray, seg);
    if (!o) continue;

    if (const P* op = boost::get<P>(&*o)) {
      K::FT d = CGAL::squared_distance(*op, start);
      if (!have_hit || d < best_dist) { have_hit = true; best = *op; best_dist = d; }
    } else if (const S* os = boost::get<S>(&*o)) {
      P p1 = os->source(), p2 = os->target();
      K::FT d1 = CGAL::squared_distance(start, p1);
      K::FT d2 = CGAL::squared_distance(start, p2);
      P nearer = (d1 < d2 ? p1 : p2);
      K::FT dn = (d1 < d2 ? d1 : d2);
      if (!have_hit || dn < best_dist) { have_hit = true; best = nearer; best_dist = dn; }
    }
  }

  if (!have_hit) {
    cout << "no" << endl;
  } else {
    cout << floor_to_double(best.x()) << " "
         << floor_to_double(best.y()) << endl;
  }
}

int main(){
  ios::sync_with_stdio(false); cin.tie(nullptr);
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  int n; cin>>n;
  while (n!=0) { 
    testcase(n); 
    cin >> n; 
  }
}
