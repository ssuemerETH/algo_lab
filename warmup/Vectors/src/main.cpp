#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> a;
  a.reserve(n);
  for (int i = 0; i < n; i++) {
    int ai; std::cin >> ai;
    a.push_back(ai);
  }
  
  int d; std::cin >> d;
  int l, r; std::cin >> l >> r;
  a.erase(a.begin() + d);
  a.erase(a.begin() + l, a.begin() + r + 1);
  
  if (a.empty())
    std::cout << "Empty\n";
  else {
    for (int x: a) std::cout << x << " ";
    std::cout << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}