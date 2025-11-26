#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::All_faces_iterator Face_iterator;

void testcase(int n) {
  Triangulation t;
  std::vector<K::Point_2> pts; pts.reserve(n);
  for (int i = 0; i < n; i++) {
    long xi, yi; std::cin >> xi >> yi;
    pts.push_back(K::Point_2(xi, yi));
  }

  t.insert(pts.begin(), pts.end());

  for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f)
    if (!t.is_infinite(f)) std::cout << t.triangle(f) << "\n";
    else std::cout << "Found infinite face\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n); std::cin >> n;
  }

  return 0;
}
