#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef std::vector<std::vector<int>> IM;
typedef std::vector<std::vector<bool>> BM;
typedef std::vector<std::vector<int>> ADJ;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property,
boost::property<boost::edge_weight_t, int>> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

void compute_maxs(const IM& c, const ADJ& adj, IM& maxs, int original, int cur, int acc) {
  maxs[original][cur] = acc;
  for (int v: adj[cur]) {
    if (maxs[original][v] > -2) continue;
    int cur_cost = c[cur][v];
    int new_acc = std::max(acc, cur_cost);
    compute_maxs(c, adj, maxs, original, v, new_acc);
  }
}

void testcase() {
  int n, tat; std::cin >> n >> tat;
  graph G(n);
  IM c(n, std::vector<int>(n));
  for (int u = 0; u < n - 1; u++) 
    for (int v = u + 1; v < n; v++) {
      int ci; std::cin >> ci;
      boost::add_edge(u, v, ci, G);
      c[u][v] = c[v][u] = ci;
    }

  std::vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

  ADJ adj(n, std::vector<int>());
  BM mst_edge(n, std::vector<bool>(n, false));
  int mst_cost = 0;
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); it++) {
    int u = boost::source(*it, G); int v = boost::target(*it, G);
    adj[u].push_back(v); adj[v].push_back(u);
    mst_edge[u][v] = mst_edge[v][u] = true;
    mst_cost += c[u][v];
  }

  IM maxs(n, std::vector<int>(n, -2));
  for (int s = 0; s < n; s++) compute_maxs(c, adj, maxs, s, s, -1);

  int min_diff = -1;
  for (int u = 0; u < n; u++)
    for (int v = u + 1; v < n; v++) {
      if (!mst_edge[u][v]) {
	int cur_diff = c[u][v] - maxs[u][v];
	if (min_diff == -1 || cur_diff < min_diff) min_diff = cur_diff;
      }
    }

  std::cout << mst_cost + min_diff << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
