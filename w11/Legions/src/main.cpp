#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

ET floor_ET(CGAL::Quotient<ET> x) {
  ET num = x.numerator(); ET den = x.denominator();
  ET floor_value = num / den;  // Integer division in Gmpz truncates toward 0
  // But to ensure it's true floor:
  if ((num < 0) && (num % den != 0)) 
    floor_value -= 1;

  return floor_value;
}

void testcase() {
  long xs, ys; int n;
  std::cin >> xs >> ys >> n;
  Program lp(CGAL::SMALLER, false, 0, false, 0);
  int px = 0; int py = 1; int t = 2; lp.set_l(t, true, 0); // t >= 0
  for (int i = 0; i < n; i++) {
    long a, b, c, v; std::cin >> a >> b >> c >> v;
    long norm = std::sqrt(a * a + b * b);
    if (a * xs + b * ys < -c) {
      lp.set_a(px, i, a); lp.set_a(py, i, b); lp.set_a(t, i, norm * v); lp.set_b(i, -c);
    } else {
      lp.set_a(px, i, -a); lp.set_a(py, i, -b); lp.set_a(t, i, norm * v); lp.set_b(i, c);
    }
  }

  lp.set_c(t, -1);
  Solution s = CGAL::solve_linear_program(lp, ET());
  std::cout << floor_ET(-s.objective_value()) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
