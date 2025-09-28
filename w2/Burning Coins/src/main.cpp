#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> IM;

int solve(bool my_turn, int left, int right, IM& first_dp, IM& second_dp, const std::vector<int>& v) {
  if (my_turn && first_dp[left][right] != -1) return first_dp[left][right];
  if (!my_turn && second_dp[left][right] != -1) return second_dp[left][right];

  if (left == right) return my_turn ? v[left] : 0;
  
  if (my_turn) {
    first_dp[left][right] = v[left] + solve(false, left + 1, right, first_dp, second_dp, v);
    first_dp[left][right] = std::max(first_dp[left][right], v[right] + solve(false, left, right - 1, first_dp, second_dp, v));
    return first_dp[left][right];
  } else {
    second_dp[left][right] = solve(true, left + 1, right, first_dp, second_dp, v);
    second_dp[left][right] = std::min(second_dp[left][right], solve(true, left, right - 1, first_dp, second_dp, v));
    return second_dp[left][right];
  }
}

void testcase() {
  int n; std::cin >> n;
  std::vector<int> v; v.reserve(n);
  for (int i = 0; i < n; i++) {
    int vi; std::cin >> vi;
    v.push_back(vi);
  }

  IM first_dp(n, std::vector<int>(n, -1)), second_dp(n, std::vector<int>(n, -1));
  std::cout << solve(true, 0, n - 1, first_dp, second_dp, v) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
