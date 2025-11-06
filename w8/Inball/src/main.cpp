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

void testcase(int n, int d) {
  Program lp(CGAL::SMALLER, false, 0, false, 0);

  // variables 0, ..., d - 1 are coordinates of center, variable d is the radius
  for (int i = 0; i < n; i++) {
    int normsq = 0;
    for (int j = 0; j < d; j++) {
      int aij; std::cin >> aij;
      normsq += aij * aij;
      lp.set_a(j, i, aij);
    }

    int bi; std::cin >> bi;
    lp.set_a(d, i, (int) std::sqrt(normsq));
    lp.set_b(i, bi);
  }

  lp.set_a(d, n, -1); lp.set_b(n, 0);
  lp.set_c(d, -1);

  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal())
    std::cout << floor_ET(-s.objective_value()) << "\n";
  else if (s.is_infeasible())
    std::cout << "none\n";
  else
    std::cout << "inf\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n, d; std::cin >> n;
  while (n != 0) {
    std::cin >> d;
    testcase(n, d);
    std::cin >> n;
  }
}
