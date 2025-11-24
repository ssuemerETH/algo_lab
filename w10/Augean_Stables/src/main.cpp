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

bool solve_for_fixed_a_p(const std::vector<std::vector<long>> &params, long a, long p) {
  Program lp(CGAL::LARGER, true, 0, true, 1);
  const int h1 = 0;
  const int h2 = 1;
  const int h3 = 2;

  int n = params[0].size();
  for (int i = 0; i < n; i++) {
    lp.set_a(h1, i, params[2][i] + a * a);
    lp.set_a(h2, i, params[3][i] + p * p);
    lp.set_a(h3, i, params[4][i] + a * p);
    lp.set_b(i, params[0][i] - params[1][i]);
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  return !s.is_infeasible();
}

int solve_for_fixed_p(const std::vector<std::vector<long>> &params, const std::vector<long> &a, long p) {
  int la = 0; int ra = 24;
  while (la < ra) {
    int ma = (la + ra) / 2;
    bool res = solve_for_fixed_a_p(params, a[ma], p);
    if (res) ra = ma;
    else la = ma + 1;
  }

  if (solve_for_fixed_a_p(params, a[la], p)) return la;
  else return -1;
}

// for some fixed a, returns the smallest number of hours for p for which the problem is feasible.
// If no such p exists, returns -1
int solve_for_fixed_a(const std::vector<std::vector<long>> &params, const std::vector<long> &p, long a) {
  int lp = 0; int rp = 24;
  while (lp < rp) {
    int mp = (lp + rp) / 2;
    bool res = solve_for_fixed_a_p(params, a, p[mp]);
    if (res) rp = mp;
    else lp = mp + 1;
  }

  if (solve_for_fixed_a_p(params, a, p[lp])) return lp;
  else return -1;
}

void testcase() {
  int n; std::cin >> n;
  std::vector<long> f, c, k, l, m;
  f.reserve(n); c.reserve(n); k.reserve(n); l.reserve(n); m.reserve(n);
  for (int i = 0; i < n; i++) {
    long fi, ci, ki, li, mi; std::cin >> fi >> ci >> ki >> li >> mi;
    f.push_back(fi); c.push_back(ci); k.push_back(ki); l.push_back(li); m.push_back(mi);
  }

  std::vector<long> a, p; a.reserve(25); p.reserve(25);
  a.push_back(0); p.push_back(0);
  for (int i = 1; i <= 24; i++) {
    long ai; std::cin >> ai;
    a.push_back(a[i - 1] + ai);
  }

  for (int i = 1; i <= 24; i++) {
    long pi; std::cin >> pi;
    p.push_back(p[i - 1] + pi);
  }
  
  std::vector<std::vector<long>> params = {f, c, k, l, m};

  // determine smallest a, for which there is some p that makes the problem feasible
  int la = 0; int ra = 24;
  while (la < ra) {
    int ma = (la + ra) / 2;
    int res = solve_for_fixed_a(params, p, a[ma]);
    if (res != -1) ra = ma;
    else la = ma + 1;
  }

  int min_p_for_min_feasible_a = solve_for_fixed_a(params, p, a[la]);
  if (min_p_for_min_feasible_a == -1) {
    std::cout << "Impossible!\n";
    return;
  }

  // at this point, la is the smallest a, for which there is some p that makes the problem feasible,
  // and the smallest such p is min_p_for_min_feasible_a

  // now, determine smallest p, for which there is some a that makes the problem feasible
  int lp = 0; int rp = 24;
  while (lp < rp) {
    int mp = (lp + rp) / 2;
    int res = solve_for_fixed_p(params, a, p[mp]);
    if (res != -1) rp = mp;
    else lp = mp + 1;
  }

  int min_a_for_min_feasible_p = solve_for_fixed_p(params, a, p[lp]);
  // no need for check here

  // at this point, lp is the smallest p, for which there is some a that makes the problem feasible,
  // and the smallest such a is min_a_for_min_feasible_p

  int min_a = std::min(la, min_a_for_min_feasible_p);
  int min_p = std::min(lp, min_p_for_min_feasible_a);
  int max_a = std::max(la, min_a_for_min_feasible_p);
  int max_p = std::max(lp, min_p_for_min_feasible_a);

  int best_total = std::min(la + min_p_for_min_feasible_a, lp + min_a_for_min_feasible_p);
  
  for (int ha = min_a; ha <= max_a; ha++)
    for (int hp = min_p; hp <= max_p; hp++) 
      if (ha + hp < best_total && solve_for_fixed_a_p(params, a[ha], p[hp])) best_total = ha + hp;
    
  std::cout << best_total << "\n";
  
  /*
  int min_val = 49;
  for (int ha = 0; ha <= 24; ha++) {
    if (min_val <= ha) break;
    int p_max = min_val - ha - 1;
    int res = solve_for_fixed_a(params, p, a[ha], p_max);
    if (res != -1) min_val = std::min(ha + res, min_val);
  }

  if (min_val < 49) std::cout << min_val << "\n";
  else std::cout << "Impossible!\n";
  */
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
