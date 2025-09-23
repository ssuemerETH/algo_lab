#include <iostream>
#include <set>

void testcase() {
  int q; std::cin >> q;
  std::set<int> s;
  
  for (int i = 0; i < q; i++) {
    int a, b; std::cin >> a >> b;
    if (a == 0) s.insert(b);
    else {
      std::set<int>::iterator it = s.find(b);
      if (it != s.end())
        s.erase(it);
    }
  }
  
  if (s.empty()) std::cout << "Empty\n";
  else {
    for (int x: s) std::cout << x << " ";
    std::cout << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) testcase();
  return 0;
}