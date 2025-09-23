#include <iostream>
#include <queue>
#include <vector>

typedef std::vector<std::vector<int>> ADJ;

void testcase() {
  int n, m, v; std::cin >> n >> m >> v;
  ADJ adj(n, std::vector<int>());
  for (; m > 0; m--) {
    int a, b; std::cin >> a >> b;
    adj[a].push_back(b);
    adj[b].push_back(a);
  } 
  
  std::queue<int> q;
  std::vector<int> dist(n, -1);
  
  q.push(v);
  dist[v] = 0;
  while (!q.empty()) {
    int x = q.front();
    q.pop();
    for (int y: adj[x])
      if (dist[y] == -1) {
        dist[y] = dist[x] + 1;
        q.push(y);
      }
  }
  
  for (int d: dist) std::cout << d << " ";
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}