///1
#include <iostream>
#include <map> 
#include <vector>
#include <set>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef std::vector<std::vector<int>> IM;
typedef std::map<std::string, std::set<int>> II; // inverted index
typedef boost::adjacency_list<boost::vecS,
boost::vecS,
boost::undirectedS> graph;

typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

void testcase() {
  int n, c, f; std::cin >> n >> c >> f;
  IM w(n, std::vector<int>(n, 0));
  II char_to_stu;
  
  for (int i = 0; i < n; i++)
    for (int j = 0; j < c; j++) {
      std::string ch; std::cin >> ch;
      if (char_to_stu.find(ch) == char_to_stu.end()) 
        char_to_stu[ch] = std::set<int>();
      char_to_stu[ch].insert(i); 
    }
  
  for (std::map<std::string, std::set<int>>::iterator it = char_to_stu.begin(); it != char_to_stu.end(); it++) 
    for (int x: it->second)
      for (int y: it->second)
        if (x < y) w[x][y]++;
  
  graph G(n);
  for (int i = 0; i < n; i++) 
    for (int j = i + 1; j < n; j++)
      if (w[i][j] > f) boost::add_edge(i, j, G);
  
  std::vector<vertex_desc> mate(n);
  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate.begin(),
    boost::get(boost::vertex_index, G)));
  
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate.begin(),
    boost::get(boost::vertex_index, G)));
  
  if (2 * matching_size == n) std::cout << "not optimal\n";
  else std::cout << "optimal\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}