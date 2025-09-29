#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> IM;

void testcase() {
  int n, m, k; std::cin >> n >> m >> k;
  std::vector<int> v; v.reserve(n);
  for (int i = 0; i < n; i++) {
    int vi; std::cin >> vi;
    v.push_back(vi);
  }

  // std::cout << "preloop\n";
  // for every index i in {0, ..., n - 1} compute the unique index i' >= i s.t.
  // sum(v[i...i']) = k if it exists and -1 if it does not.
  std::vector<int> ksum(n, -1);
  int sum = v[0];
  int i, j; i = j = 0;
  while (j < n) {
    if (sum == k) {
      ksum[i] = j;
      j++;
      if (j < n) sum += v[j];
    } else if (sum < k) {
      j++;
      if (j < n) sum += v[j];
    } else {
      sum -= v[i];
      i++;
      if (i > j) {
	j++;
	if (j < n) sum += v[j];
      }
    }

    // std::cout << "loop\n";
  }
  
  IM dp(n + 1, std::vector<int>(m + 1, -1));

  // if there are q members left, feasible plan exists but has value zero.
  for (int i = 0; i <= n; i++) dp[i][0] = 0;

  for (int i = n - 1; i >= 0; i--)
    for (int q = 1; q <= m; q++) {
      int res = -1;
      int ip = ksum[i];
      if (ip != -1 && dp[ip + 1][q - 1] != -1) res = (ip - i + 1) + dp[ip + 1][q - 1];
      res = std::max(res, dp[i + 1][q]);
      dp[i][q] = res;
    }

  int fin = dp[0][m];
  if (fin == -1) std::cout << "fail\n";
  else std::cout << fin << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
