#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> VVI;

struct PrefixInfo {
  int waterway_index, length;

  bool operator<(const PrefixInfo &other) const {
    return length < other.length;
  }
};

void testcase() {
  int n, k, w; std::cin >> n >> k >> w;
  std::vector<int> c(n);
  for (int i = 0; i < n; i++) std::cin >> c[i];

  // waterways, containing c-values, not island indices
  // Pyke not included in any waterway
  VVI waterway(w, std::vector<int>());
  for (int i = 0; i < w; i++) {
    int l; std::cin >> l;
    int pyke_c; std::cin >> pyke_c;
    for (int j = 0; j < l - 1; j++) {
      int rij; std::cin >> rij;
      waterway[i].push_back(c[rij]);
    }
  }

  // find the optimal solution not containing Pyke
  int res = 0;
  for (int i = 0; i < w; i++) {
    int l = 0; int r = 0;
    int sum = waterway[i][0];
    int ni = waterway[i].size();
    while (r < ni) {
      if (sum == k) {
	res = std::max(res, r - l + 1);
	r++;
	if (r < ni) sum += waterway[i][r];
      } else if (sum < k) {
	r++;
	if (r < ni) sum += waterway[i][r];
      } else {
	sum -= waterway[i][l];
	l++;
	if (l > r) {
	  r++;
	  if (r < ni) sum += waterway[i][r];
	}
      }
    }
  
  }

  // find the optimal solution containing Pyke
  if (c[0] > k) {
    std::cout << res << "\n";
    return;
  }

  k -= c[0];
  std::vector<PrefixInfo> best_pref_info(k + 1, PrefixInfo{-1, -n - 1});
  std::vector<PrefixInfo> snd_best_pref_info(k + 1, PrefixInfo{-1, -n - 1});
  best_pref_info[0] = PrefixInfo{0, 0};
  snd_best_pref_info[0] = PrefixInfo{1, 0};
  
  for (int i = 0; i < w; i++) {
    int sum = 0;
    int ni = waterway[i].size();
    for (int j = 0; j < ni; j++) {
      sum += waterway[i][j];
      PrefixInfo pref{i, j + 1};
      if (sum <= k) {
	if (best_pref_info[sum] < pref) {
	  snd_best_pref_info[sum] = best_pref_info[sum];
	  best_pref_info[sum] = pref;
	} else if (snd_best_pref_info[sum] < pref)
	  snd_best_pref_info[sum] = pref;
      } else break;
    }
  }

  for (int q = 0; q <= k; q++) {
    PrefixInfo best_q = best_pref_info[q];
    PrefixInfo snd_best_q = snd_best_pref_info[q];

    PrefixInfo best_qc = best_pref_info[k - q];
    PrefixInfo snd_best_qc = snd_best_pref_info[k - q];
    
    if (best_q.waterway_index != best_qc.waterway_index)
      res = std::max(res, best_q.length + best_qc.length + 1);
    else {
      res = std::max(res, best_q.length + snd_best_qc.length + 1);
      res = std::max(res, snd_best_q.length + best_qc.length + 1);
    }
  }

  std::cout << res << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
