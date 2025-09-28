#include <iostream>
#include <vector>

typedef std::vector<std::vector<std::pair<int, long>>> ADJ;
typedef std::vector<std::vector<long>> LM;

void testcase() {
  int n, m, k; long x; std::cin >> n >> m >> x >> k;
  ADJ adj(n, std::vector<std::pair<int, long>>());
  for (int i = 0; i < m; i++) {
    int u, v; long p; std::cin >> u >> v >> p;
    adj[u].push_back(std::make_pair(v, p));
  }

  // dp[i][l] is the maximum number of possible achievable points starting at i with l moves
  LM dp(n, std::vector<long>(k + 1, -1L));
  for (int i = 0; i < n; i++) dp[i][0] = 0L;

  for (int l = 1; l <= k; l++) 
    for (int i = 0; i < n; i++) 
      if (adj[i].empty()) dp[i][l] = dp[0][l];
      else for (const std::pair<int, long>& p: adj[i]) dp[i][l] = std::max(dp[i][l], p.second + dp[p.first][l - 1]);
  
  for (int m = 0; m <= k; m++) 
    if (dp[0][m] >= x) {
      std::cout << m << "\n";
      return;
    }

  std::cout << "Impossible\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
