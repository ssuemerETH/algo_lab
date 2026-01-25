#include <iostream>
#include <vector>

void testcase() {
  int n, m, k; std::cin >> n >> m >> k;
  std::vector<int> horcrux(n);
  for (int i = 0; i < n; i++) std::cin >> horcrux[i];
  std::vector<int> k_pointer(n, -1);

  int l = 0; int r = 0;
  int sum = horcrux[0];
  while (r < n) {
    if (sum == k) {
      k_pointer[l] = r;
      sum -= horcrux[l];
      l++;
      if (l > r) {
	r++;
	if (r < n) sum += horcrux[r];
      }
    } else if (sum < k) {
      r++;
      if (r < n) sum += horcrux[r];
    } else {
      sum -= horcrux[l];
      l++;
      if (l > r) {
	r++;
	if (r < n) sum += horcrux[r];
      }
    }
  }

  std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, -1));
  for (int i = 0; i <= n; i++) dp[i][0] = 0;
  for (int i = n - 1; i >= 0; i--)
    for (int q = 1; q <= m; q++) {
      int res = dp[i + 1][q];
      if (k_pointer[i] != -1) {
	int cur_eff = k_pointer[i] - i + 1;
	int rec_res = dp[k_pointer[i] + 1][q - 1];
	if (rec_res != -1)
	  res = std::max(res, cur_eff + rec_res);
      }

      dp[i][q] = res;
    }

  int res = dp[0][m];
  if (res == -1) std::cout << "fail\n";
  else std::cout << res << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
