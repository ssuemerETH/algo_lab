#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> h;
  h.reserve(n);
  for (int i = 0; i < n; i++) {
    int hi; std::cin >> hi;
    h.push_back(hi);
  }
  
  int res = 0;
  for (int i = 0; i <= res && i < n; i++) res = std::max(res, i + h[i] - 1);
  
  std::cout << std::min(res, n - 1) + 1 << "\n";
}

int main() {
  int t; std::cin >> t;
  for (; t > 0; t--) 
    testcase();
  return 0;
}