#include <iostream>
#include <string>
#include <vector>
// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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

int enc(int i, int j, int w) { return i * w + j; }

void testcase() {
  int h, w; std::cin >> h >> w;
  int nc = 'Z' - 'A' + 1;
  std::string note; std::cin >> note;
  IM front(h, std::vector<int>(w)), num_occ(nc, std::vector<int>(nc));
  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {
      char c; std::cin >> c;
      front[i][j] = c - 'A';
    }

  for (int i = 0; i < h; i++)
    for (int j = w - 1; j >= 0; j--) {
      char c; std::cin >> c;
      num_occ[front[i][j]][c - 'A']++;
    }

  // 0..(nc * nc - 1): product vertices, (nc * nc), ..., (nc * nc + nc - 1): char vertices
  graph G(nc * nc + nc + 2); edge_adder adder(G);
  int source = nc * nc + nc; int sink = nc * nc + nc + 1;
  for (int i = 0; i < nc; i++)
    for (int j = 0; j < nc; j++) {
      int cur = enc(i, j, nc);
      int occ = num_occ[i][j];
      adder.add_edge(source, cur, occ);
      adder.add_edge(cur, nc * nc + i, occ);
      adder.add_edge(cur, nc * nc + j, occ);
    }

  std::vector<int> note_occ(nc);
  for (char c: note) note_occ[c - 'A']++;
  for (int i = 0; i < nc; i++) adder.add_edge(nc * nc + i, sink, note_occ[i]);
  long flow = boost::push_relabel_max_flow(G, source, sink);

  std::cout << (flow == note.size() ? "Yes\n" : "No\n");
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (; t > 0; t--) testcase();
  return 0;
}
