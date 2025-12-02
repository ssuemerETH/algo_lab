#include <iostream>
#include <vector>

typedef std::vector<std::vector<long>> LM;

void testcase() {
  int n, m; std::cin >> n >> m;
  LM r(n, std::vector<long>(m, 0));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) 
      std::cin >> r[i][j];

  LM vpref(n, std::vector<long>(m, 0));
  for (int j = 0; j < m; j++) {
    vpref[0][j] = r[0][j];
    for (int i = 1; i < n; i++)
      vpref[i][j] = vpref[i - 1][j] + r[i][j];
  }

  LM hpref(n, std::vector<long>(m, 0));
  for (int i = 0; i < n; i++) {
    hpref[i][0] = r[i][0];
    for (int j = 1; j < m; j++)
      hpref[i][j] = hpref[i][j - 1] + r[i][j];
  }

  LM vsum(n, std::vector<long>(m, 0));
  for (int j = 0; j < m; j++)
    for (int i = 1; i < n; i++)
      vsum[i][j] = vpref[i - 1][j] + vsum[i - 1][j];

  LM hsum(n, std::vector<long>(m, 0));
  for (int i = 0; i < n; i++)
    for (int j = 1; j < m; j++)
      hsum[i][j] = hpref[i][j - 1] + hsum[i][j - 1];
  
  LM dp(n, std::vector<long>(m, 0));
  for (int i = 1; i < n; i++)
    for (int j = 1; j < m; j++) {
      // need to arrive at (i, j)

      // assume coming from (i - 1, j)
      long res_1 = dp[i - 1][j] + hsum[i][j];

      // assume coming from (i, j - 1)
      long res_2 = dp[i][j - 1] + vsum[i][j];

      dp[i][j] = std::min(res_1, res_2);
    }

  std::cout << dp[n - 1][m - 1] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
