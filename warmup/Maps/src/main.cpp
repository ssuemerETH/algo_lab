#include <iostream>
#include <map>
#include <string>
#include <set>

void testcase() {
  int q; std::cin >> q;
  std::multimap<std::string, int> m;
  
  for (int i = 0; i < q; i++) {
    int a; std::string b; std::cin >> a >> b;
    if (a == 0) m.erase(b);
    else m.insert(std::make_pair(b, a));
  }
  
  std::string s; std::cin >> s;
  auto range = m.equal_range(s);
  if (range.first == range.second) std::cout << "Empty";
  else {
    std::set<int> res;
    for (auto i = range.first; i != range.second; i++) res.insert(i->second);
    for (int x: res) std::cout << x << " ";
  }
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) testcase();
  return 0;
}