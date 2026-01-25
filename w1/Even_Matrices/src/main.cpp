#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> IM;

void testcase() {
  int n; std::cin >> n;
  IM pref; pref.reserve(n);
  for (int i = 0; i < n; i++) {
    pref.push_back(std::vector<int>());
    pref[i].reserve(n + 1);
    pref[i].push_back(0);
    for (int j = 0; j < n; j++) {
      int xij; std::cin >> xij;
      pref[i].push_back(xij + pref[i][j]);
    }
  }

  int res = 0;
  for (int j1 = 0; j1 <= n; j1++)
    for (int j2 = j1 + 1; j2 <= n; j2++) {
      int even_pref = 1;
      int sum = 0;
      for (int i = 0; i < n; i++) {
	sum += pref[i][j2] - pref[i][j1];
	if (sum % 2 == 0) even_pref++;
      }

      int odd_pref = n + 1 - even_pref;
      res += even_pref * (even_pref - 1) / 2;
      res += odd_pref * (odd_pref - 1) / 2;
    }

  std::cout << res << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
