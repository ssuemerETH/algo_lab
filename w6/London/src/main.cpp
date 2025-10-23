#include <iostream>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef std::vector<std::vector<int>> IM;

class edge_adder {
  graph &G;
  public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int encode(int i, int j, int q) {return i * q + j;}

void testcase() {
  int h, w; std::cin >> h >> w;
  int alph_size = 'Z' - 'A' + 1;
  std::vector<int> wc_note(alph_size, 0);
  std::string note; std::cin >> note;
  for (char c: note)
    wc_note[c - 'A']++;

  IM front(h, std::vector<int>(w, 0)), back(h, std::vector<int>(w, 0));
  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {
      char c; std::cin >> c;
      front[i][j] = c - 'A';
    }

  for (int i = 0; i < h; i++)
    for (int j = w - 1; j >= 0; j--) {
      char c; std::cin >> c;
      back[i][j] = c - 'A';
    }

  // tiles[i][j] is the number of tiles available that has i on the front side and j on the back side
  IM tiles(alph_size, std::vector<int>(alph_size, 0));
  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++)
      tiles[front[i][j]][back[i][j]]++;

  
  // vertex for tile of type (i, j) at index encode(i, j, alph_size)
  // vertex for letter i is at alph_size * alph_size + i
  // source at alph_size * alph_size + alph_size
  // sink at alph_size * alph_size + alph_size + 1
  int tile_types = alph_size * alph_size;
  graph G(2 + alph_size + tile_types);
  edge_adder adder(G);
  int source = tile_types + alph_size;
  int sink = tile_types + alph_size + 1;
  
  for (int i = 0; i < alph_size; i++)
    for (int j = 0; j < alph_size; j++) {
      int cur = encode(i, j, alph_size);
      adder.add_edge(source, cur, tiles[i][j]);
      adder.add_edge(cur, tile_types + i, tiles[i][j]);
      if (i != j) adder.add_edge(cur, tile_types + j, tiles[i][j]);
    }

  for (int i = 0; i < alph_size; i++) adder.add_edge(tile_types + i, sink, wc_note[i]);
  long flow = boost::push_relabel_max_flow(G, source, sink);
  if (flow == note.size()) std::cout << "Yes\n";
  else std::cout << "No\n";
}
  
int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
