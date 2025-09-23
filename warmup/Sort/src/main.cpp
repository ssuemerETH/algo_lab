#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> a;
  a.reserve(n);
  for (; n > 0; n--) {
    int ai; std::cin >> ai;
    a.push_back(ai);
  }
  
  int x; std::cin >> x;
  if (x == 0) std::sort(a.begin(), a.end());
  else std::sort(a.begin(), a.end(), std::greater<int>());
  
  for (int ai: a) std::cout << ai << " ";
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}