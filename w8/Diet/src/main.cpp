#include <iostream>
#include <vector>
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

void testcase(int n, int m) {
  std::vector<int> min_nut, max_nut; min_nut.reserve(n); max_nut.reserve(n);
  for (int i = 0; i < n; i++) {
    int mi, Mi; std::cin >> mi >> Mi;
    min_nut.push_back(mi); max_nut.push_back(Mi);
  }

  // A[i][j] is the amount of nutrient i in food j
  std::vector<std::vector<int>> A(n, std::vector<int>(m));
  std::vector<int> p; p.reserve(m);
  for (int j = 0; j < m; j++) {
    int pj; std::cin >> pj; p.push_back(pj);
    for (int i = 0; i < n; i++) 
      std::cin >> A[i][j];
  }

  Program lp(CGAL::SMALLER, true, 0, false, 0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      lp.set_a(j, i, A[i][j]);
      lp.set_a(j, n + i, -A[i][j]);
    }
    lp.set_b(i, max_nut[i]); lp.set_b(n + i, -min_nut[i]);
  }

  for (int j = 0; j < m; j++) lp.set_c(j, p[j]);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal()) std::cout << floor_ET(s.objective_value()) << "\n";
  else if (s.is_infeasible()) std::cout << "No such diet.\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n, m; std::cin >> n >> m;
  while (n != 0) {
    testcase(n, m); std::cin >> n >> m;
  }
}
