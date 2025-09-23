#include <iostream>
#include <string>

void reverse(std::string& x) {
  int n = x.size();
  for (int i = 0; 2 * i < n - 1; i++) {
    char f = x[i];
    x[i] = x[n - 1 - i];
    x[n - 1 - i] = f;
  }
}

void testcase() {
  std::string a, b; std::cin >> a >> b;
  std::cout << a.size() << " " << b.size() << "\n";
  std::cout << (a + b) << "\n";
  
  reverse(a); reverse(b);
  char fa = a[0];
  a[0] = b[0];
  b[0] = fa;
  
  std::cout << a << " " << b << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}