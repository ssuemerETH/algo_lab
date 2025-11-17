#include <iostream>

void testcase() {
  int n; std::cin >> n;
  int res = 0;
  for (int i = 0; i < n; i++) {
    int x; std::cin >> x;
    res += x;
  }
  
  std::cout << res << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}