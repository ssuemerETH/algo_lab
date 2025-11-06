#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef std::pair<long, long> P2D;

ET ceil_ET(CGAL::Quotient<ET> x) {
  ET num = x.numerator(); ET den = x.denominator();
  ET ceil_value = num / den;  // Truncates toward 0
  if ((num > 0) && (num % den != 0)) 
    ceil_value += 1;
  return ceil_value;
}

void testcase() {
  int n, m; std::cin >> n >> m;
  long s; std::cin >> s;
  std::vector<P2D> nobles, commons; nobles.reserve(n); commons.reserve(m);
  long Nx, Ny, Cx, Cy; Nx = Ny = Cx = Cy = 0;
  
  for (int i = 0; i < n; i++) {
    long x, y; std::cin >> x >> y;
    nobles.push_back(std::make_pair(x, y));
    Nx += x; Ny += y;
  }

  for (int i = 0; i < m; i++) {
    long x, y; std::cin >> x >> y;
    commons.push_back(std::make_pair(x, y));
    Cx += x; Cy += y;
  }

  // Equation for the water canal: ax + y + c_w = 0, where a, c_w are variables
  // Equation for the sewage canal: -x + ay + c_s = 0, where a, c_s are variables

  const int a = 0;
  const int cw = 1;
  const int cs = 2;
  
  // First, model Cersei's constraints
  Program lp(CGAL::SMALLER, false, 0, false, 0);

  // counter for the current constraint index
  // make sure to increment it after every new constraint added
  int cc = 0;
  
  // For any common (x, y), need: -x + ay + c_s <= 0
  // equivalent to: ya + 0c_w + 1c_s <= x
  for (P2D &n: commons) {
    long x = n.first; long y = n.second;
    lp.set_a(a, cc, y); lp.set_a(cw, cc, 0); lp.set_a(cs, cc, 1); lp.set_b(cc, x); cc++;
  }

  // For any noble (x, y), need: -x + ay + c_s >= 0
  // equivalent to: ya + 0c_w + 1c_s >= x
  // equivalent to: -ya + 0c_w -1c_s <= -x
  for (P2D &n: nobles) {
    long x = n.first; long y = n.second;
    lp.set_a(a, cc, -y); lp.set_a(cw, cc, 0); lp.set_a(cs, cc, -1); lp.set_b(cc, -x); cc++;
  }

  Solution sol = CGAL::solve_linear_program(lp, ET());
  if (sol.is_infeasible()) {std::cout << "Y\n"; return;}
  
  if (s != -1) {
    lp.set_a(a, cc, Ny - Cy); lp.set_a(cw, cc, 0); lp.set_a(cs, cc, n - m); lp.set_b(cc, s + Nx - Cx); cc++;
  }

  const int d = 3;
  lp.set_c(d, 1);

  for (P2D &n: commons) {
    long x = n.first; long y = n.second;
    lp.set_a(a, cc, x); lp.set_a(cw, cc, 1); lp.set_a(cs, cc, 0); lp.set_a(d, cc, -1); lp.set_b(cc, -y); cc++;
    lp.set_a(a, cc, -x); lp.set_a(cw, cc, -1); lp.set_a(cs, cc, 0); lp.set_a(d, cc, -1); lp.set_b(cc, y); cc++;
  }

  for (P2D &n: nobles) {
   long x = n.first; long y = n.second;
    lp.set_a(a, cc, x); lp.set_a(cw, cc, 1); lp.set_a(cs, cc, 0); lp.set_a(d, cc, -1); lp.set_b(cc, -y); cc++;
    lp.set_a(a, cc, -x); lp.set_a(cw, cc, -1); lp.set_a(cs, cc, 0); lp.set_a(d, cc, -1); lp.set_b(cc, y); cc++;
  }
  
  sol = CGAL::solve_linear_program(lp, ET());
  if (sol.is_infeasible()) {
      std::cout << "B\n";
      return;
  } else if (sol.is_optimal()) {
    std::cout << ceil_ET(sol.objective_value()) << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
