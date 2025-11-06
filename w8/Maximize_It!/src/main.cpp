#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
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

ET ceil_ET(CGAL::Quotient<ET> x) {
  ET num = x.numerator(); ET den = x.denominator();
  ET ceil_value = num / den;  // Truncates toward 0
  if ((num > 0) && (num % den != 0)) 
    ceil_value += 1;
  return ceil_value;
}

void solve1(int a, int b) {
  Program lp(CGAL::SMALLER, true, 0, false, 0);
  const int X = 0; const int Y = 1;
  lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
  lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b);
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);

  lp.set_c(X, a); lp.set_c(Y, -b);
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal()) 
    std::cout << floor_ET(-s.objective_value()) << "\n";
  else if (s.is_infeasible())
    std::cout << "no\n";
  else if (s.is_unbounded())
    std::cout << "unbounded\n";
}

void solve2(int a, int b) {
  Program lp(CGAL::LARGER, false, 0, true, 0);
  const int X = 0; const int Y = 1; const int Z = 2;
  lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);
  lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a * b);
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);

  lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal())
    std::cout << ceil_ET(s.objective_value()) << "\n";
  else if (s.is_infeasible())
    std::cout << "no\n";
  else if (s.is_unbounded())
    std::cout << "unbounded\n";
}

void testcase(int p, int a, int b) {
  if (p == 1) solve1(a, b);
  else solve2(a, b);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int p, a, b; std::cin >> p;
  while (p != 0) {
    std::cin >> a >> b;
    testcase(p, a, b);
    std::cin >> p;
  }
}
