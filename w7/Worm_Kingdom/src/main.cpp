#include <iostream>
#include <vector>
#include <set>
#include <cassert>

typedef std::vector<std::vector<int>> ADJ;

void compute_subtree_sizes(int s, const ADJ &adj, const std::vector<int> &sizes, std::vector<int> &out) {
  int res = sizes[s];
  for (int v: adj[s]) {
    compute_subtree_sizes(v, adj, sizes, out);
    res += out[v];
  }
  out[s] = res;
  assert(res > 0);
}

void testcase() {
  int k, p; std::cin >> k >> p;
  // adj[i] is the adjacency list for the i-th tree
  std::vector<ADJ> adjs; adjs.reserve(k);

  // ts[i][j] is the size of the j-th room of the i-th tree
  std::vector<std::vector<int>> ts; ts.reserve(k);

  // ss[i][j] is the total size of the subtree rooted at vertex j of tree i
  std::vector<std::vector<int>> ss; ss.reserve(k);
  
  for (int i = 0; i < k; i++) {
    int ni; std::cin >> ni; // size of i-th tree
    adjs.push_back(ADJ(ni, std::vector<int>()));
    
    ts.push_back(std::vector<int>());
    ts[i].reserve(ni);

    ss.push_back(std::vector<int>(ni));
    
    for (int j = 0; j < ni - 1; j++) {
      int u, v; std::cin >> u >> v;
      adjs[i][u].push_back(v);
    }

    for (int j = 0; j < ni; j++) {
      int sj; std::cin >> sj;
      ts[i].push_back(sj);
    }

    compute_subtree_sizes(0, adjs[i], ts[i], ss[i]);
  }

  int M = 0;
  for (int i = 0; i < k; i++)
    M += ss[i][0];

  // dp[i][j] == true iff there is a partitioning of the values 0, ..., i yielding an absolute difference of j
  std::vector<std::vector<bool>> dp(k, std::vector<bool>(M + 1, false));
  dp[0][ss[0][0]] = true;
  for (int i = 1; i < k; i++) {
    int cur = ss[i][0];
    for (int j = 0; j <= M; j++)
      dp[i][j] = dp[i - 1][std::abs(j - cur)] || dp[i - 1][std::abs(j + cur)];
  }

  std::vector<int> possible_diff;
  for (int j = 0; j <= M; j++)
    if (dp[k - 1][j]) possible_diff.push_back(j);
  
  int min = possible_diff[0];
  for (int i = 0; i < k; i++)
    for (int j = 1; j < ss[i].size(); j++) {
      int flip1 = ss[i][j];
      std::vector<int>::iterator lb = std::lower_bound(possible_diff.begin(), possible_diff.end(), 2 * flip1);
      if (lb != possible_diff.end())
	min = std::min(min, *lb - 2 * flip1);
      if (lb != possible_diff.begin())
	min = std::min(min, 2 * flip1 - *(lb - 1));

      int flip2 = ss[i][0] - ss[i][j];
      lb = std::lower_bound(possible_diff.begin(), possible_diff.end(), 2 * flip2);
      if (lb != possible_diff.end())
	min = std::min(min, *lb - 2 * flip2);
      if (lb != possible_diff.begin())
	min = std::min(min, 2 * flip2 - *(lb - 1));
    }

  std::cout << min + p * (k - 1) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
       
