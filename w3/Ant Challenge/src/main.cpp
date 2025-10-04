#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <vector>
#include <cassert>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property,
boost::property<boost::edge_weight_t, int>
> w_graph;

typedef boost::graph_traits<w_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<w_graph>::edge_descriptor edge_desc;

void testcase() {
  int n, m, s, a, b; std::cin >> n >> m >> s >> a >> b;
  std::vector<w_graph> networks(s, w_graph(n));
  for (int i = 0; i < m; i++) {
    int u, v; std::cin >> u >> v;
    for (int q = 0; q < s; q++) {
      int c; std::cin >> c;
      boost::add_edge(u, v, c, networks[q]);
    }
  }
  
  std::vector<int> h; h.reserve(n);
  for (int i = 0; i < s; i++) {
    int hi; std::cin >> hi;
    h.push_back(hi);
  }
  
  w_graph spg(n);
  auto spg_weight_map = boost::get(boost::edge_weight, spg);
  for (int i = 0; i < s; i++) {
    // find MST in networks[i] via Prim, for each found MST edge, relax the weight of it in spg
    std::vector<vertex_desc> parent(n);
    boost::prim_minimum_spanning_tree(
        networks[i],
        &parent[0],
        boost::root_vertex(h[i]).
        weight_map(boost::get(boost::edge_weight, networks[i])).
        vertex_index_map(boost::get(boost::vertex_index, networks[i]))
    );
    
    auto weight_map = boost::get(boost::edge_weight, networks[i]);
    for (int j = 0; j < n; j++) {
      if (j == h[i]) continue; // hive won't have a parent in the MST
      std::pair<edge_desc, bool> e = boost::edge(j, parent[j], networks[i]);
      assert(e.second);
      int w = weight_map[e.first];
      
      // check if edge already exists in spg
      std::pair<edge_desc, bool> e_spg = boost::edge(j, parent[j], spg);
      if (e_spg.second) spg_weight_map[e_spg.first] = std::min(spg_weight_map[e_spg.first], w);
      else boost::add_edge(j, parent[j], w, spg);
    }
  }
  
  std::vector<int> dist(n);
  
  boost::dijkstra_shortest_paths(spg, a,
    boost::distance_map(boost::make_iterator_property_map(dist.begin(),
    boost::get(boost::vertex_index, spg))));
  
  std::cout << dist[b] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}