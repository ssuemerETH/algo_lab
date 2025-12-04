#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Quotient.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Edge {
  int i, j, d;
  CGAL::Gmpq e;
};

CGAL::Gmpz ceil_ET(CGAL::Quotient<ET> x) {
  ET f_num = x.numerator(); ET f_den = x.denominator();
  CGAL::Gmpz num = f_num.numerator() * f_den.denominator();
  CGAL::Gmpz den = f_num.denominator() * f_den.numerator();
  
  CGAL::Gmpz ceil_value = num / den;  // Truncates toward 0
  if ((num > 0) && (num % den != 0)) 
    ceil_value += 1;
  return ceil_value;
}

CGAL::Gmpz solve_for_fixed_k(int a, int b, int c, const std::vector<Edge> &edges, int k, int n) {
  Program lp(CGAL::SMALLER, true, 0, false, 0);
  int m = edges.size();

  lp.set_b(0, c);
  lp.set_b(1, -b);
  for (int i = 2; i < n + m; i++) lp.set_b(i, 0);
  lp.set_b(n + m, a);
  
  for (int q = 0; q < m; q++) {
    // process q-th edge
    Edge edge = edges[q];
    int i = edge.i;
    int j = edge.j;
    int d = edge.d;
    CGAL::Gmpq e = edge.e;

    // add coefficient to suspicion constraint
    lp.set_a(q, 0, d);

    // add coefficient to Slughorn constraint
    if (j == 0)
      lp.set_a(q, 1, -e);

    // add coefficient to Harry constraint
    if (i == 1)
      lp.set_a(q, n + m, 1);

    // s-constraint or 0-constraint
    if (i < k && j < k) {
      lp.set_a(q, n + q, d);
      lp.set_a(m, n + q, -1);
    } else lp.set_a(q, n + q, 1);

    // edge is incoming for j, negative contribution to
    // flow conservation
    if (j > 1) lp.set_a(q, j, -e);

    // edge is outgoing for i, positive contribution to
    // flow conservation
    if (i > 1) lp.set_a(q, i, 1);
  }
  
  lp.set_c(m, 1);

  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal()) return ceil_ET(s.objective_value());
  else return CGAL::Gmpz(-1);
}

void testcase() {
  int n, m, a, b, c; std::cin >> n >> m >> a >> b >> c;
  std::vector<Edge> edges; edges.reserve(m);
  for (int k = 0; k < m; k++) {
    int i, j, d, e_num, e_denom; std::cin >> i >> j >> d >> e_num >> e_denom;
    edges.push_back({i - 1, j - 1, d, CGAL::Gmpq(e_num, e_denom)});
  }

  int l = 2; int r = n;
  while (l < r) {
    int mid = (l + r) / 2;
    CGAL::Gmpz res = solve_for_fixed_k(a, b, c, edges, mid, n);
    if (res == -1) l = m + 1;
    else r = m;
  }

  CGAL::Gmpz res = solve_for_fixed_k(a, b, c, edges, l, n);
  if (res == -1) std::cout << "Busted!\n";
  else std::cout << l << " " << res << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
