#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<
boost::vecS,
boost::vecS,
boost::undirectedS,
boost::no_property, // no vertex property
boost::property <boost::edge_weight_t, int> // edge property (interior)
> w_graph;

typedef boost::graph_traits<w_graph>::edge_descriptor edge_desc;

void testcase() {
  int n, m; std::cin >> n >> m;
  w_graph G(n);
  for (int i = 0; i < m; i++) {
    int u, v, c; std::cin >> u >> v >> c;
    boost::add_edge(u, v, c, G);
  }
  
  std::vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  auto weight_map = boost::get(boost::edge_weight, G);
  int mst_total = 0;
  for (const edge_desc& e: mst)
    mst_total += weight_map[e];
  
  std::vector<int> dist(n);
  boost::dijkstra_shortest_paths(G, 0,
boost::distance_map(boost::make_iterator_property_map(dist.begin(),
boost::get(boost::vertex_index, G))));

  int max_dist = dist[0];
  for (int i = 1; i < n; i++) max_dist = std::max(max_dist, dist[i]);
  
  std::cout << mst_total << " " << max_dist << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
