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

  LM dp(n, std::vector<long>(k + 1, 0));
  for (int c = 1; c <= k; c++) 
    for (int i = 0; i < n; i++) {
      if (adj[i].empty()) dp[i][c] = dp[0][c];
      else {
	for (const std::pair<int, long> &p: adj[i]) {
	  int j = p.first; long reward = p.second;
	  dp[i][c] = std::max(dp[i][c], reward + dp[j][c - 1]);
	}
      }
    }

  for (int c = 0; c <= k; c++)
    if (dp[0][c] >= x) {
      std::cout << c << "\n";
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
