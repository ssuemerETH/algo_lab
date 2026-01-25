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
boost::property <boost::edge_weight_t, int> // edge property ( interior )
> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

void testcase() {
  int n, m; std::cin >> n >> m;
  graph G(n);
  for (int i = 0; i < m; i++) {
    int u, v, w; std::cin >> u >> v >> w;
    boost::add_edge(u, v, w, G);
  }

  std::vector<edge_desc> mst_edges;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst_edges));
  int mst_weight = 0;
  auto weight_map = boost::get(boost::edge_weight, G);
  for (const edge_desc &edge: mst_edges)
    mst_weight += weight_map[edge];

  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, 0,
   boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
		       boost::get(boost::vertex_index, G))));

  int max_dist = 0;
  for (int dist: dist_map) max_dist = std::max(max_dist, dist);

  std::cout << mst_weight << " " << max_dist << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
