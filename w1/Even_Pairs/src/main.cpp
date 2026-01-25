#include <iostream>

void testcase() {
  int n; std::cin >> n;
  int even_prefs = 1;
  int sum = 0;
  for (int i = 0; i < n; i++) {
    int xi; std::cin >> xi;
    sum += xi;
    if (sum % 2 == 0) even_prefs++;
  }

  int odd_prefs = n + 1 - even_prefs;
  std::cout << (even_prefs * (even_prefs - 1) / 2 + odd_prefs * (odd_prefs - 1) / 2) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
