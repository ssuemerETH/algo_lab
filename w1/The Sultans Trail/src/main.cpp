#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
  int n; std::cin >> n;
  std::vector<std::pair<int, int>> p; p.reserve(2 * n);
  for (int i = 0; i < n; i++) {
    int li, ri; std::cin >> li >> ri;
    p.push_back(std::make_pair(li, 1));
    p.push_back(std::make_pair(ri + 1, -1));
  }

  std::sort(p.begin(), p.end());

  int max, sum; max = sum = 0;
  for (int i = 0; i < 2 * n; i++) {
    sum += p[i].second;
    max = std::max(max, sum);
  }

  std::cout << max << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
