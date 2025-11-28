#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

typedef std::vector<std::vector<int>> ADJ;

void testcase() {
  int n; std::cin >> n;
  ADJ adj(n, std::vector<int>());
  for (int i = 0; i < n - 1; i++) {
    int u, v; std::cin >> u >> v;
    adj[u].push_back(v);
  }

  std::vector<int> c; c.reserve(n);
  for (int i = 0; i < n; i++) {
    int ci; std::cin >> ci;
    c.push_back(ci);
  }

  std::vector<int> order; order.reserve(n);
  order.push_back(0);
  std::queue<int> q;
  q.push(0); 
  while (!q.empty()) {
    int x = q.front(); q.pop();
    for (int v: adj[x]) {
      q.push(v); order.push_back(v);
    }
  }

  std::reverse(order.begin(), order.end());
  
  std::vector<int> ldp(n), dp(n), udp(n);
  for (int v: order) {
    // ldp[v]
    int res_with_v = c[v];
    for (int w: adj[v]) res_with_v += ldp[w];
    int res_without_v = 0;
    for (int w: adj[v]) res_without_v += dp[w];
    ldp[v] = std::min(res_with_v, res_without_v);

    // dp[v]
    if (!adj[v].empty()) {
      int total = 0;
      int min_diff = -1;
      for (int w: adj[v]) {
	total += dp[w];
	int cur_diff = udp[w] - dp[w];
	if (min_diff == -1 || cur_diff < min_diff) min_diff = cur_diff;
      }

      dp[v] = std::min(res_with_v, total + min_diff);
    } else dp[v] = res_with_v;

    // udp[v]
    udp[v] = res_with_v;
  }

  std::cout << dp[0] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
