#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef Triangulation::Finite_vertices_iterator Vertex_iterator;


void visit(const Triangulation::Vertex_handle &vh, const Triangulation &t, long r, int cc, std::vector<int> &component) {
  
  int ind = vh->info();
  component[ind] = cc;
  
  Triangulation::Vertex_circulator vc = t.incident_vertices(vh);
  Triangulation::Vertex_circulator done = vc;
  K::Point_2 cur = vh->point();
  int dim = t.dimension();

  if (dim == 2) {
    do {
      if (!t.is_infinite(vc)) {
	K::Point_2 other = vc->point();
	int other_ind = vc->info();
	if (CGAL::squared_distance(cur, other) <= r * r && component[other_ind] == -1)
	  visit(vc, t, r, cc, component);
      }
    } while (++vc != done);
  } else {
    Triangulation::Edge_circulator ec = t.incident_edges(vh);
    Triangulation::Edge_circulator done = ec;

    do {
      Triangulation::Vertex_handle u = ec->first->vertex(t.cw(ec->second));
      Triangulation::Vertex_handle v = ec->first->vertex(t.ccw(ec->second));
      if (CGAL::squared_distance(cur, u->point()) <= r * r && component[u->info()] == -1)
	visit(u, t, r, cc, component);
      if (CGAL::squared_distance(cur, v->point()) <= r * r && component[v->info()] == -1)
	visit(v, t, r, cc, component);
    } while (++ec != done);
  }
}

bool solve_for_fixed_k(int k, const std::vector<K::Point_2> &pts, long r) {
  Triangulation t; t.insert(pts.begin() + k, pts.end());
  std::vector<int> component(pts.size() - k, -1);
  int counter = 0;
  for (Vertex_iterator vit = t.finite_vertices_begin(); vit != t.finite_vertices_end(); vit++)
    vit->info() = counter++;

  int component_counter = 0;
  for (Vertex_iterator vit = t.finite_vertices_begin(); vit != t.finite_vertices_end(); vit++) {
    int ind = vit->info();
    if (component[ind] == -1) {
      visit(vit, t, r, component_counter, component);
      component_counter++;
    }
  }

  std::vector<int> count(component_counter, 0);
  for (int c: component) {
      count[c]++;
    if (count[c] >= k) return true;
  }

  return false;
}

void testcase() {
  int n; long R; std::cin >> n >> R;
  std::vector<K::Point_2> pts; pts.reserve(n);
  for (int i = 0; i < n; i++) {
    long x, y; std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }

  /*
  for (int i = n / 2; i >= 1; i--)
    if (solve_for_fixed_k(i, pts, R)) {
      std::cout << i << "\n";
      return;
    }
  */
  
  int l = 1; int r = n / 2;
  while (l < r) {
    int m = (l + r) / 2;
    bool res = solve_for_fixed_k(m, pts, R);
    if (res) l = m + 1;
    else r = m;
  }

  if (solve_for_fixed_k(l, pts, R)) std::cout << l << "\n";
  else std::cout << l - 1 << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
