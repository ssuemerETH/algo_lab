#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::pair<int, bool>> IBV;

void testcase() {
  int n; std::cin >> n;
  IBV events; events.reserve(2 * n);
  for (int i = 0; i < n; i++) {
    int l, r; std::cin >> l >> r;
    events.push_back(std::make_pair(l, true));
    events.push_back(std::make_pair(r + 1, false));
  }

  std::sort(events.begin(), events.end());
  int max = 0;
  int cur = 0;
  for (int i = 0; i < 2 * n; i++) {
    bool beg = events[i].second;
    if (beg) cur++;
    else cur--;
    max = std::max(max, cur);
  }

  std::cout << max << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
