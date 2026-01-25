#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::pair<int, int>> PV;

void testcase() {
  int n; std::cin >> n;
  std::vector<int> m(n);
  for (int i = 0; i < n; i++) std::cin >> m[i];
  PV positions;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m[i]; j++) {
      int pij; std::cin >> pij;
      positions.push_back(std::make_pair(pij, i));
    }

  std::sort(positions.begin(), positions.end());
  int res = -1;
  int l = 0; int r = 0;
  int cur_count = 1;
  std::vector<int> word_count(n, 0);
  word_count[positions[0].second]++;
  int total_pos = positions.size();
  
  while (r < total_pos) {
    int cur_length = positions[r].first - positions[l].first + 1;
    if (cur_count == n) {
      if (res == -1 || cur_length < res) 
	res = cur_length;
      word_count[positions[l].second]--;
      if (word_count[positions[l].second] == 0) cur_count--;
      l++;
      if (l > r) {
	r++;
	if (r < total_pos) {
	  word_count[positions[r].second]++;
	  if (word_count[positions[r].second] == 1) cur_count++;
	}
      }
    } else {
      r++;
      if (r < total_pos) {
	word_count[positions[r].second]++;
	if (word_count[positions[r].second] == 1) cur_count++;
      }
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
