#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> IM;
typedef std::pair<int, int> IP;
typedef std::vector<IP> PV;

int p_compare(const IP& i1_best, const IP& i1_sec_best, const IP& i2_best, const IP& i2_sec_best) {
  if (i1_best.second == -1 || i2_best.second == -1) return -1;

  if (i1_best.second != i2_best.second) return i1_best.first + i2_best.first;
  int res = -1;

  if (i2_sec_best.second != -1) res = i1_best.first + i2_sec_best.first;
  if (i1_sec_best.second != -1) res = std::max(res, i1_sec_best.first + i2_best.first);
  return res;
}

int max_k(const std::vector<int>& arr, int k) {
  int n = arr.size();
  int i, j; i = j = 0;
  int sum = arr[i];
  int res = -1;

  while (j < n) {
    if (sum == k) {
      res = std::max(res, j - i + 1);
      j++;
      if (j < n) sum += arr[j];
    } else if (sum < k) {
      j++;
      if (j < n) sum += arr[j];
    } else {
      sum -= arr[i];
      i++;
      if (i > j) {
	j++;
	if (j < n) sum += arr[j];
      }
    }
  }

  return res;
}

void testcase() {
  int n, k, w; std::cin >> n >> k >> w;
  std::vector<int> c; c.reserve(n);
  for (int i = 0; i < n; i++) {
    int ci; std::cin >> ci;
    c.push_back(ci);
  }

  IM ww(w, std::vector<int>());
  
  // read waterways
  for (int i = 0; i < w; i++) {
    int l, pyke; std::cin >> l >> pyke;
    ww[i].reserve(l - 1);
    for (int j = 0; j < l - 1; j++) {
      int rj; std::cin >> rj;
      ww[i].push_back(c[rj]);
    }
  }

  int single_res = -1;
  // for each waterway, find the length of a largest contiguous subarray summing to k
  for (int i = 0; i < w; i++) single_res = std::max(single_res, max_k(ww[i], k));

  if (c[0] > k) {
    if (single_res == -1) std::cout << 0 << "\n";
    else std::cout << single_res << "\n";
    return;
  }

  k -= c[0];

  // find two waterways s.t. they have prefix sums that sum to k AND the total lengths of these prefixes is maximised.
  // for each i in {0, ..., k} we will record the two waterways that have some prefix summing to i and s.t. these prefixes are longest possible
  PV best(k + 1, std::make_pair(-1, -1)), sec_best(k + 1, std::make_pair(-1, -1));
  best[0] = std::make_pair(0, 0); sec_best[0] = std::make_pair(0, 1);

  for (int i = 0; i < w; i++) {
    int sum = 0;
    for (int j = 0; j < ww[i].size(); j++) {
      sum += ww[i][j];
      if (sum > k) break;
      std::pair<int, int> cur(j + 1, i);
      if (cur > best[sum]) {
	sec_best[sum] = best[sum];
	best[sum] = cur;
      } else if (cur > sec_best[sum]) sec_best[sum] = cur;
    }
  }

  int pyke_res = -1;
  for (int q = 0; q <= k; q++) {
    int cur_comb = p_compare(best[q], sec_best[q], best[k - q], sec_best[k - q]);
    if (cur_comb != -1) pyke_res = std::max(pyke_res, 1 + cur_comb);
  }

  int res = std::max(single_res, pyke_res);
  if (res == -1) std::cout << 0 << "\n";
  else std::cout << res << "\n";
}



int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
