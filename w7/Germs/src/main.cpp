#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <algorithm>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

long death_time(long R) {
  if (R <= 1) return 0;
  double q = 0.5 * (std::sqrt(R) - 1);
  double t = std::sqrt(q);
  return (long) std::ceil(t);
}

void testcase(int n) {
  long l, b, r, t; std::cin >> l >> b >> r >> t;
  std::vector<K::Point_2> pts; pts.reserve(n);
  
  for (int i = 0; i < n; i++) {
    long xi, yi; std::cin >> xi >> yi;
    pts.push_back(K::Point_2(xi, yi));
  }

  Triangulation tri;
  tri.insert(pts.begin(), pts.end());
  std::vector<long> Rs; Rs.reserve(n);
  for (auto u = tri.finite_vertices_begin(); u != tri.finite_vertices_end(); u++) {
    K::Point_2 pu = u->point();
    long x = pu.x(); long y = pu.y();
    long res = x - l;
    res = std::min(res, r - x);
    res = std::min(res, y - b);
    res = std::min(res, t - y);
    res = 4 * res * res;

    if (tri.dimension() >= 1) {
      auto v = tri.incident_vertices(u);
      auto vc = v;
      do {
	if (!tri.is_infinite(v)) {
	  K::Point_2 pv = v->point();
	  res = std::min(res, (long) CGAL::squared_distance(pu, pv));
	}
      } while (++v != vc);
    }
    
    Rs.push_back(res);
  }

  std::sort(Rs.begin(), Rs.end());
  
  long first = death_time(Rs[0]);
  long mid = death_time(Rs[n / 2]);
  long last = death_time(Rs[n - 1]);

  std::cout << first << " " << mid << " " << last << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n); std::cin >> n;
  }
  return 0;
}
