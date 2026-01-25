#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<std::vector<long>>> M3D;

void testcase() {
  int n, m; std::cin >> n >> m;
  long a, b; std::cin >> a >> b;
  long P, H, W; std::cin >> P >> H >> W;
  std::vector<long> p(n), h(n), w(m);
  for (int i = 0; i < n; i++) std::cin >> p[i] >> h[i];
  for (int i = 0; i < m; i++) std::cin >> w[i];

  std::sort(w.begin(), w.end(), std::greater<long>());
  std::vector<long> w_pref(m + 1);
  for (int i = 1; i <= m; i++) w_pref[i] = w_pref[i - 1] + w[i - 1];

  M3D dp(n, std::vector<std::vector<long>>(n + 1, std::vector<long>(H + 1, -1L)));
  for (int i = 0; i < n; i++) dp[i][0][0] = 0L;
  for (int k = 1; k <= n; k++)
    for (int q = 0; q <= std::min(h[0], H); q++)
      dp[0][k][q] = p[0];

  for (int i = 1; i < n; i++)
    for (int k = 1; k <= n; k++)
      for (long q = 0; q <= H; q++) {
	long res = -1;
	
	long res_use = dp[i - 1][k - 1][std::max(0L, q - h[i])];
	if (res_use != -1) res = p[i] + res_use;
	
	long res_no_use = dp[i - 1][k][q];
	res = std::max(res, res_no_use);
	dp[i][k][q] = res;
      }

  int ans = n + m + 1;
  for (int i = 0; i <= m; i++) {
    long W_new = w_pref[i] - W;
    if (W_new < 0) continue;
    
    long pot_limit = (a == 0 ? n : std::min(W_new / a, (long) n));
    for (int k = 0; k <= pot_limit; k++) {
      if (dp[n - 1][k][H] >= P + b * i) {
	ans = std::min(ans, i + k);
	break;
      }
    }
  }

  if (ans > n + m) ans = -1;
  std::cout << ans << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
