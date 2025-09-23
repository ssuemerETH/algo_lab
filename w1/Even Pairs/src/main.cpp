#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> x; x.reserve(n);
  for (int i = 0; i < n; i++) {
    int xi; std::cin >> xi;
    x.push_back(xi);
  }
  
  int even_c = 1;
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += x[i];
    if (sum % 2 == 0) even_c++;
  }
  
  int odd_c = n + 1 - even_c;
  int res = even_c * (even_c - 1) / 2 + odd_c * (odd_c - 1) / 2;
  std::cout << res << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}