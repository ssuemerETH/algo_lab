#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> a, b; a.reserve(n); b.reserve(n);
  for (int i = 0; i < n; i++) {
    int ai; std::cin >> ai;
    a.push_back(ai - 1);
  }

  for (int i = 0; i < n; i++) {
    int bi; std::cin >> bi;
    b.push_back(bi - 1);
  }
  
  std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, -1));
  dp[n][n] = 0;
  for (int i = n - 1; i >= 0; i--)
    for (int j = n - 1; j >= 0; j--) {
      // pick a[i] to be singleton in match
      // iterate over all subarrays of form b[k..(n - 1)]
      int cur = 0;
      for (int q = j; q < n; q++) {
	cur += a[i] * b[q];
	int sub_res = dp[i + 1][q + 1];
	if (sub_res != -1 && (dp[i][j] == -1 || dp[i][j] > cur + sub_res)) dp[i][j] = cur + sub_res;
      }

      cur = 0;
      for (int q = i; q < n; q++) {
	cur += b[j] * a[q];
	int sub_res = dp[q + 1][j + 1];
	if (sub_res != -1 && (dp[i][j] == -1 || dp[i][j] > cur + sub_res)) dp[i][j] = cur + sub_res;
      }
    }

  std::cout << dp[0][0] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
