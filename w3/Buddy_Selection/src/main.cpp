#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

void testcase() {
  int n, c, f; std::cin >> n >> c >> f;
  std::map<std::string, std::vector<int>> inv_index;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < c; j++) {
      std::string ch; std::cin >> ch;
      if (inv_index.find(ch) == inv_index.end()) inv_index[ch] = std::vector<int>();
      inv_index[ch].push_back(i);
    }

  std::vector<std::vector<int>> common_char(n, std::vector<int>(n));
  for (const auto& [ch, vec]: inv_index) {
    int len = vec.size();
    for (int i = 0; i < len; i++)
      for (int j = i + 1; j < len; j++) {
	int su = vec[i];
	int sv = vec[j];
	if (su > sv) std::swap(su, sv);
	common_char[su][sv]++;
      }
  }

  graph G(n);
  for (int u = 0; u < n; u++)
    for (int v = u + 1; v < n; v++)
      if (common_char[u][v] > f) boost::add_edge(u, v, G);

  std::vector<vertex_desc> mate_map(n);
  boost::edmonds_maximum_cardinality_matching(G,
	boost::make_iterator_property_map(mate_map.begin(),
        boost::get(boost::vertex_index, G)));
  
  int matching_size = boost::matching_size(G,
        boost::make_iterator_property_map(mate_map.begin(),
        boost::get(boost::vertex_index, G)));

  if (matching_size == n / 2) std::cout << "not optimal\n";
  else std::cout << "optimal\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
