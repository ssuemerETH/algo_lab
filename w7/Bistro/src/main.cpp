#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

void testcase(int n) {
  std::vector<K::Point_2> p; p.reserve(n);
  for (int i = 0; i < n; i++) {
    long xi, yi; std::cin >> xi >> yi;
    p.push_back(K::Point_2(xi, yi));
  }

  Triangulation t;
  t.insert(p.begin(), p.end());

  int m; std::cin >> m;
  for (int i = 0; i < m; i++) {
    long xi, yi; std::cin >> xi >> yi;
    K::Point_2 cur(xi, yi);
    K::Point_2 nearest = t.nearest_vertex(cur)->point();
    long dist = CGAL::to_double(CGAL::squared_distance(cur, nearest));
    std::cout << std::fixed << std::setprecision(0) << dist << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n); std::cin >> n;
  }
}
