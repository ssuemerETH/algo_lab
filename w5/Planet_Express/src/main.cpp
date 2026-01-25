#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>

typedef boost::adjacency_list<
boost::vecS,
boost::vecS,
boost::directedS,
boost::no_property, // no vertex property
boost::property <boost::edge_weight_t, int > // edge property ( interior )
> graph;

void testcase() {
  int n, m, k, T; std::cin >> n >> m >> k >> T;
  std::vector<bool> is_tp(n);
  for (int i = 0; i < T; i++) {
    int ti; std::cin >> ti;
    is_tp[ti] = true;
  }

  graph G(n);
  for (int i = 0; i < m; i++) {
    int u, v, c; std::cin >> u >> v >> c;
    boost::add_edge(v, u, c, G);
  }

  std::vector<int> scc_map(n);
  int nscc = boost::strong_components(G,
boost::make_iterator_property_map(scc_map.begin(),
boost::get(boost::vertex_index, G)));

  // n..(n + nscc - 1): scc vertices
  for (int i = 0; i < nscc; i++) boost::add_vertex(G);

  std::vector<int> scc_size(nscc);
  for (int i = 0; i < n; i++)
    if (is_tp[i]) scc_size[scc_map[i]]++;

  for (int i = 0; i < n; i++) 
    if (is_tp[i]) {
      int cur_scc = scc_map[i];
      boost::add_edge(i, n + cur_scc, scc_size[cur_scc] - 1, G);
      boost::add_edge(n + cur_scc, i, 0, G);
    }

  std::vector<int> dist_map(n + nscc);
  boost::dijkstra_shortest_paths(G, n - 1,
boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
boost::get(boost::vertex_index, G))));

  int res = 1000001;
  for (int i = 0; i < k; i++)
    res = std::min(res, dist_map[i]);

  if (res <= 1000000) std::cout << res << "\n";
  else std::cout << "no\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
