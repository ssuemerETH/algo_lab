#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::pair<long, long>> PV;
typedef std::vector<std::vector<std::vector<long>>> LM;

void testcase() {
  int n, m; std::cin >> n >> m;
  long a, b; std::cin >> a >> b;
  long P, H, W; std::cin >> P >> H >> W;
  
  PV A; A.reserve(n);
  for (int i = 0; i < n; i++) {
    long api, ahi; std::cin >> api >> ahi;
    A.push_back(std::make_pair(api, ahi));
  }
  
  std::vector<long> B; B.reserve(m);
  for (int i = 0; i < m; i++) {
    long bwi; std::cin >> bwi;
    B.push_back(bwi);
  }
  
  // dp[i][k][h] = max. achievable power using A[0...i] with at most k potions (of type A) granting at least h happiness (-1 if no subset of k potions among A[0...i] grants at least h happiness)
  LM dp(n, std::vector<std::vector<long>>(n + 1, std::vector<long>(H + 1, -1)));
  
  // DP table initialization:
  
  // with at most 0 potions and a lower bound of 0 happiness, a maximum power of 0 is possible.
  for (int i = 0; i < n; i++) dp[i][0][0] = 0;
  
  // if only A[0] is available and it may be used, we check whether it yields enough happiness
  for (int k = 1; k <= n; k++)
    for (long h = 0; h <= std::min(H, A[0].second); h++) dp[0][k][h] = A[0].first;
      
  // inductive phase: i -> i + 1, keep k > 0
  for (int i = 1; i < n; i++)
    for (int k = 1; k <= n; k++)
      for (long h = 0; h <= H; h++) {
        // use A[i]
        long rec = dp[i - 1][k - 1][std::max(0L, h - A[i].second)];
        // if happiness of max(0, h - A[i].h) possible, update
        if (rec != -1) dp[i][k][h] = A[i].first + rec;
        // don't use A[i]
        dp[i][k][h] = std::max(dp[i][k][h], dp[i - 1][k][h]);
      }
  
  std::sort(B.begin(), B.end(), std::greater<long>());
  
  long res = n + m + 1;
  long gathered_wit = 0;
  for (int i = 0; i < m; i++) {
    gathered_wit += B[i];
    if (gathered_wit < W) continue;
    long allowed_a_pots = std::min(a > 0 ? (gathered_wit - W) / a : n, (long) n);
    
    // drank i + 1 potions of type B, so need an additional power of (i + 1) * b while drinking potions of type A
    long needed_power = P + (i + 1) * b;
  
    // determine minimum k in the range {0, ..., allowed_a_pots} s.t. it grants at least H happiness and yields a max. achievable power of needed_power
    for (long k = 0; k <= allowed_a_pots; k++)
      if (dp[n - 1][k][H] >= needed_power) { 
        res = std::min(res, i + 1 + k);
        break;
      }
  }
  
  std::cout << (res == n + m + 1 ? -1 : res) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}