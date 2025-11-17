#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

// For each subset of potions, compute their total d and t values and also their sizes
// We need the sizes so that for a given s_i value, total_d can be adjusted accordingly
struct SubsetInfo {
  long total_d;
  long total_t;
  int size;
  
  // For sorting
  bool operator<(const SubsetInfo& other) const {
    return total_t < other.total_t;
  }
};

long solve(const std::vector<SubsetInfo> &subsets1, const std::vector<SubsetInfo> &subsets2, long si, long T) {
  int n2 = subsets2.size();
  
  // max_d[i] is the maximum value of total_d + size * si in subsets2[0..i]
  std::vector<long> max_d; max_d.reserve(n2);
  max_d.push_back(subsets2[0].total_d + subsets2[0].size * si);
  for (int i = 1; i < n2; i++) max_d.push_back(std::max(max_d[i - 1], subsets2[i].total_d + subsets2[i].size * si));
  
  long max = -1;
  for (const SubsetInfo &i1: subsets1) {
    long cur_score = i1.total_d + i1.size * si;
    if (i1.total_t >= T) continue;
    long rem_t = T - 1 - i1.total_t;
    
    auto it = std::upper_bound(subsets2.begin(), subsets2.end(), SubsetInfo{0L, rem_t, 0});
    if (it == subsets2.begin()) continue;
    it--; // now points to last element of subsets2 for which total_t <= rem_t
    int q = it - subsets2.begin();
    max = std::max(max, cur_score + max_d[q]);
  }
  
  return max;
}

void testcase() {
  int n, m; long D, T;
  std::cin >> n >> m >> D >> T;
  std::vector<long> d, t; d.reserve(n); t.reserve(n);
  for (int i = 0; i < n; i++) {
    long di, ti; std::cin >> di >> ti;
    d.push_back(di); t.push_back(ti);
  }
  
  std::vector<long> s; s.reserve(m + 1);
  // taking 0 gulps is also possible, yielding no bonus per move
  s.push_back(0);
  for (int i = 0; i < m; i++) {
    long si; std::cin >> si;
    s.push_back(si);
  }
  
  // use split & list, we have n1, n2 <= 15
  int n1 = n / 2;
  int n2 = n - n1;
  
  // Compute all subsets of both halves
  std::vector<SubsetInfo> subsets1; subsets1.reserve(1 << n1);
  std::vector<SubsetInfo> subsets2; subsets2.reserve(1 << n2);
  
  
  for (int i = 0; i < 1 << n1; i++) {
    SubsetInfo si{};
    for (int j = 0; j < n1; j++) 
      if (i & 1 << j) {
        si.total_d += d[j];
        si.total_t += t[j];
        si.size++;
      }
    
    subsets1.push_back(si);
  }
  
  for (int i = 0; i < 1 << n2; i++) {
    SubsetInfo si{};
    for (int j = 0; j < n2; j++) 
      if (i & 1 << j) {
        si.total_d += d[n1 + j];
        si.total_t += t[n1 + j];
        si.size++;
      }
    
    subsets2.push_back(si);
  }
  
  std::sort(subsets2.begin(), subsets2.end());
  
  // Do binary search over the number of gulps, find smallest value that 
  // yields a solution
  int l = 0; int r = m;
  while (l < r) {
    int middle = (l + r) / 2;
    if (solve(subsets1, subsets2, s[middle], T) >= D) r = middle;
    else l = middle + 1;
  }
  
  if (solve(subsets1, subsets2, s[l], T) >= D) std::cout << l << "\n";
  else std::cout << "Panoramix captured\n";
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
