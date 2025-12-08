#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

void testcase() {
  int a, s, b; std::cin >> a >> s >> b;
  int e; std::cin >> e;

  std::vector<K::Point_2> asteroid; asteroid.reserve(a);
  std::vector<int> density; density.reserve(a);
  for (int i = 0; i < a; i++) {
    long x, y, d; std::cin >> x >> y >> d;
    asteroid.push_back(K::Point_2(x, y));
    density.push_back(d);
  }
  
  std::vector<K::Point_2> shooting_point; shooting_point.reserve(s);
  for (int i = 0; i < s; i++) {
    long x, y; std::cin >> x >> y;
    shooting_point.push_back(K::Point_2(x, y));
  }

  std::vector<K::Point_2> bounty_hunter; bounty_hunter.reserve(b);
  for (int i = 0; i < b; i++) {
    long x, y; std::cin >> x >> y;
    bounty_hunter.push_back(K::Point_2(x, y));
  }

  Triangulation bh_t;
  bh_t.insert(bounty_hunter.begin(), bounty_hunter.end());

  // determine for each shooting point the maximum
  // permissible squared radius
  std::vector<int> sp_max_rad; sp_max_rad.reserve(s);
  if (b > 0)
    for (const K::Point_2 &sp: shooting_point) {
      Triangulation::Vertex_handle nv = bh_t.nearest_vertex(sp);
      sp_max_rad.push_back(CGAL::squared_distance(nv->point(), sp));
    }
  
  Program lp(CGAL::LARGER, true, 0, false, 0);
  
  for (int i = 0; i < a; i++)
    for (int j = 0; j < s; j++) {
      // process asteroid i and shooting point j
      long sq_dist = CGAL::squared_distance(asteroid[i], shooting_point[j]);
      // check whether asteroid in range
      if (b == 0 || sq_dist <= sp_max_rad[j]) 
	lp.set_a(j, i, CGAL::Gmpq(1, std::max(1L, sq_dist)));
    }

  for (int i = 0; i < a; i++)
    lp.set_b(i, density[i]);

  // equation at index a encodes energy constraint
  for (int j = 0; j < s; j++)
    lp.set_a(j, a, -1);
  lp.set_b(a, -e);

  Solution sol = CGAL::solve_linear_program(lp, ET());
  std::cout << (sol.is_infeasible() ? "n\n" : "y\n");
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
