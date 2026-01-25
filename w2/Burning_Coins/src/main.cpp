#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> IM;

void testcase() {
  int n; std::cin >> n;
  std::vector<int> v(n);
  for (int i = 0; i < n; i++) std::cin >> v[i];
  // dp0[i][l] is the optimal value attainable (for player 0) if player 0 starts and
  // coins at i, i + 1, ..., i + l - 1 are available

  // dp1[i][l] is the optimal value attainable (for player 0) if player 1 starts and
  // coins at i, i + 1, ..., i + l - 1 are available
  IM dp0(n + 1, std::vector<int>(n + 1)), dp1(n + 1, std::vector<int>(n + 1));

  for (int l = 1; l <= n; l++)
    for (int i = 0; i < n - l + 1; i++) {
      dp0[i][l] = std::max(v[i] + dp1[i + 1][l - 1], v[i + l - 1] + dp1[i][l - 1]);
      dp1[i][l] = std::min(dp0[i + 1][l - 1], dp0[i][l - 1]);
    }

  std::cout << dp0[0][n] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
