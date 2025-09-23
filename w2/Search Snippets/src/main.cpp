#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <set>

typedef std::pair<int, int> P;

void testcase() {
  int n; std::cin >> n;
  int total = 0;
  std::vector<int> m; m.reserve(n);
  for (int i = 0; i < n; i++) {
    int mi; std::cin >> mi;
    m.push_back(mi);
    total += mi;
  }
  
  std::vector<P> words;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m[i]; j++) {
      int pij; std::cin >> pij;
      words.push_back(std::make_pair(pij, i));
    }
  
  assert(words.size() == total);
  
  std::sort(words.begin(), words.end());
  int l = 0;
  int r = 0;
  int first_word = words[0].second;
  
  std::set<P> occ;
  std::vector<int> word_counts(n, 0);
  
  for (int i = 0; i < n; i++) {
    if (i == first_word) { occ.insert(std::make_pair(1, i)); word_counts[i] = 1; }
    else { occ.insert(std::make_pair(0, i)); word_counts[i] = 0; }
  }
  
  int res = words[total - 1].first - words[0].first + 1;
  while (!(l == r && r == total - 1)) {
    P min_occ = *occ.begin();
    if (min_occ.first >= 1) {
      res = std::min(res, words[r].first - words[l].first + 1);
      
      int old_word = words[l].second;
      int count = word_counts[old_word];
      auto it = occ.find(std::make_pair(count, old_word));
      occ.erase(it);
      occ.insert(std::make_pair(count - 1, old_word));
      word_counts[old_word]--;
      l++;
    } else if (r < total - 1) {
      r++;
      int new_word = words[r].second;
      int count = word_counts[new_word];
      auto it = occ.find(std::make_pair(count, new_word));
      occ.erase(it);
      occ.insert(std::make_pair(count + 1, new_word));
      word_counts[new_word]++;
    } else break;
  }
  
  std::cout << res << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}