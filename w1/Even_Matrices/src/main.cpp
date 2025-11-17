#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> IM;

void testcase() {
  int n; std::cin >> n;
  IM M(n, std::vector<int>());
  for (int i = 0; i < n; i++) {
    M[i].reserve(n);
    for (int j = 0; j < n; j++) {
      int mij; std::cin >> mij;
      M[i].push_back(mij);
    }
  }
  
  IM P(n, std::vector<int>());
  for (int i = 0; i < n; i++) {
    P[i].reserve(n);
    P[i].push_back(M[i][0]);
    for (int j = 1; j < n; j++) {
      P[i].push_back(P[i][j - 1] + M[i][j]);
    }
  }
  
  int total = 0;
  for (int j1 = 0; j1 < n; j1++)
    for (int j2 = j1; j2 < n; j2++) {
      int even_c = 1;
      int sum = 0;
      for (int i = 0; i < n; i++) {
        sum += P[i][j2] - (j1 > 0 ? P[i][j1 - 1] : 0);
        if (sum % 2 == 0) even_c++;
      }
      
      int odd_c = n + 1 - even_c;
      total += even_c * (even_c - 1) / 2 + odd_c * (odd_c - 1) / 2;
    }
  
  std::cout << total << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}