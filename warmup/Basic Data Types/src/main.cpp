#include <iostream>
#include <string>
#include <iomanip>

void testcase() {
  int x; std::cin >> x;
  long y; std::cin >> y;
  std::string z; std::cin >> z;
  double w; std::cin >> w;
  
  std::cout << std::setprecision(2);
  std::cout << std::fixed << x << " " << y << " " << z << " " << w << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}