#include <iostream>
#include <vector>
#include <set>

typedef std::vector<std::vector<int>> ADJ;

void fill_subtree_sizes(const std::vector<int> &s, const ADJ &adj, std::vector<int> &res, int u) {
  res[u] = s[u];
  for (int v: adj[u]) {
    fill_subtree_sizes(s, adj, res, v);
    res[u] += res[v];
  }
}

void testcase() {
  int k, p; std::cin >> k >> p;
  std::vector<std::vector<int>> s; s.reserve(k);
  std::vector<ADJ> trees; trees.reserve(k);
  std::vector<std::vector<int>> subtree_sizes; subtree_sizes.reserve(k);

  int M = 0;
  for (int i = 0; i < k; i++) {
    int ni; std::cin >> ni;
    trees.push_back(ADJ(ni, std::vector<int>()));
    for (int j = 0; j < ni - 1; j++) {
      int u, v; std::cin >> u >> v;
      trees[i][u].push_back(v);
    }

    s.push_back(std::vector<int>());
    for (int j = 0; j < ni; j++) {
      int sj; std::cin >> sj;
      s[i].push_back(sj);
      M += sj;
    }

    subtree_sizes.push_back(std::vector<int>(ni, 0));
    fill_subtree_sizes(s[i], trees[i], subtree_sizes[i], 0);
  }

  std::vector<int> values; values.reserve(k); 
  for (int i = 0; i < k; i++)
    values.push_back(subtree_sizes[i][0]);

  std::vector<std::vector<bool>> dp(k, std::vector<bool>(M + 1, false));
  dp[0][values[0]] = true;
  for (int i = 1; i < k; i++)
    for (int m = 0; m <= M; m++)
      dp[i][m] = dp[i - 1][std::abs(m - values[i])] || dp[i - 1][m + values[i]];

  std::vector<int> possible_diffs;
  for (int m = 0; m <= M; m++)
    if (dp[k - 1][m]) possible_diffs.push_back(m);
  
  std::set<int> changes_s;
  for (int i = 0; i < k; i++)
    for (int q = 0; q < trees[i].size(); q++) {
      changes_s.insert(2 * subtree_sizes[i][q]);
      changes_s.insert(2 * (subtree_sizes[i][0] - subtree_sizes[i][q]));
    }

  std::vector<int> changes; changes.reserve(changes_s.size());
  for (int x: changes_s) changes.push_back(x);

  int res = M + 1;
  int i = 0; int j = 0;
  while (i < possible_diffs.size() && j < changes.size()) {
    int x = possible_diffs[i];
    int y = changes[j];
    if (x < y) { 
      res = std::min(res, y - x);
      i++;
    } else if (x == y) {
      res = 0; break;
    } else {
      res = std::min(res, x - y);
      j++;
    }
  }
  
  std::cout << (res + (k - 1) * p) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
