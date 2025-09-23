#include <vector>
#include <set>
#include <iostream>

typedef std::vector<std::set<int>> ADJ;

void DFS_visit(int s, const ADJ& adj, std::vector<int>& pre, std::vector<int>& post, int& c) {
  pre[s] = c++;
  for (int v: adj[s]) 
    if (pre[v] == -1) DFS_visit(v, adj, pre, post, c);
  post[s] = c++;
}

void testcase() {
  int n, m, v; std::cin >> n >> m >> v;
  ADJ adj(n, std::set<int>());
  
  for (int i = 0; i < m; i++) {
    int a, b; std::cin >> a >> b;
    adj[a].insert(b);
    adj[b].insert(a);
  }
  
  std::vector<int> pre(n, -1);
  std::vector<int> post(n, -1);
  
  int c = 0;
  
  DFS_visit(v, adj, pre, post, c);
  for (int p: pre) std::cout << p << " ";
  std::cout << "\n";
  for (int p: post) std::cout << p << " ";
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; 
  std::cin >> t;
  for (int i = 0; i < t; i++) testcase();
  return 0;
}