#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <set>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

void testcase() {
  int n, m; std::cin >> n >> m;
  graph G(n);
  for (int i = 0; i < m; i++) {
    int u, v; std::cin >> u >> v;
    boost::add_edge(u, v, G);
  }
  
  std::map<edge_desc, int> component_map_storage;
  boost::associative_property_map<std::map<edge_desc, int>> component_map(component_map_storage);
  int num_comps = boost::biconnected_components(G, component_map);
  std::vector<int> edges_in_comp(num_comps);
  edge_it e_beg, e_end;
  for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; e_beg++) {
    int comp = component_map[*e_beg];
    edges_in_comp[comp]++;
  }

  std::set<std::pair<int, int>> res;
  for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; e_beg++) {
    int comp = component_map[*e_beg];
    int u = boost::source(*e_beg, G); int v = boost::target(*e_beg, G);
    if (u > v) std::swap(u, v);
    if (edges_in_comp[comp] == 1) res.insert(std::make_pair(u, v));
  }

  int k = res.size();
  std::cout << k << "\n";
  for (const std::pair<int, int> &edge: res)
    std::cout << edge.first << " " << edge.second << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
