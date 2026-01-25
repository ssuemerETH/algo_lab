#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> h; h.reserve(n);
  for (int i = 0; i < n; i++) {
    int hi; std::cin >> hi;
    h.push_back(hi);
  }
  
  int toppled_until = 1;
  int i = 0;
  for (; i < std::min(n, toppled_until); i++)
    toppled_until = std::max(toppled_until, i + h[i]);

  std::cout << i << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
