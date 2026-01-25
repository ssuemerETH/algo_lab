#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
			      boost::no_property, boost::property<boost::edge_weight_t, int>> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

void testcase() {
  int n, e, s, a, b; std::cin >> n >> e >> s >> a >> b;
  std::vector<graph> graphs(s, graph(n));
  for (int i = 0; i < e; i++) {
    int u, v; std::cin >> u >> v;
    for (int j = 0; j < s; j++) {
      int w; std::cin >> w;
      boost::add_edge(u, v, w, graphs[j]);
    }
  }

  std::vector<int> hives(s);
  for (int i = 0; i < s; i++) std::cin >> hives[i];

  std::vector<std::vector<int>> final_edges(n, std::vector<int>(n, -1));
  for (int i = 0; i < s; i++) {
    std::vector<edge_desc> mst_edges;
    boost::kruskal_minimum_spanning_tree(graphs[i], std::back_inserter(mst_edges));
    auto ew = boost::get(boost::edge_weight, graphs[i]);
    
    for (const edge_desc &edge: mst_edges) {
      int u = boost::source(edge, graphs[i]);
      int v = boost::target(edge, graphs[i]);
      int w = ew[edge];
      if (u > v) std::swap(u, v);

      if (final_edges[u][v] == -1 || w < final_edges[u][v])
	final_edges[u][v] = w;
    }
  }

  graph spg(n);
  for (int u = 0; u < n; u++)
    for (int v = u + 1; v < n; v++) {
      int w = final_edges[u][v];
      if (w != -1) boost::add_edge(u, v, w, spg);
    }

  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(spg, a,
	boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
        boost::get(boost::vertex_index, spg))));

  std::cout << dist_map[b] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
