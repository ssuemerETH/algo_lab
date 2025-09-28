#include <iostream>
#include <vector>

void testcase() {
  int n, k, w; std::cin >> n >> k >> w;
  std::vector<int> c; c.reserve(n);
  for (int i = 0; i < n; i++) {
    int ci; std::cin >> ci;
    c.push_back(ci);
  }

  // read waterways
}



int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
